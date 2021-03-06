#include "controller.h"
#include <mpd/client.h>
#include <QDebug>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QTcpSocket>

Controller::Controller(QString host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
    , m_host(host)
    , m_port(port)
    , m_timeout_ms(timeout_ms)
    , m_connection(nullptr)
    , m_notifier(nullptr)
{
    qRegisterMetaType<Controller::ConnectionState>();
}

void Controller::handleConnectClick()
{
    emit connectionState(ConnectionState::Connecting);

    // This is literally how libmpdclient determines if a host is a Unix socket,
    // at least as of 2.18.
    if (m_host.startsWith("/") || m_host.startsWith("@")) {
        createMPD();

    } else {
        // If it's a Tcp socket, then we first use Qt to asynchronously check if we can actually
        // connect, because libmpdclient's internal host address resolution is blocking and
        // can take a long time to return.
        auto socket = new QTcpSocket();

        connect(socket, &QTcpSocket::errorOccurred, [=](QTcpSocket::SocketError error) {
            Q_UNUSED(error);
            emit errorMessage(socket->errorString());
            socket->deleteLater();
            emit connectionState(ConnectionState::Disconnected);
        });

        connect(socket, &QTcpSocket::connected, [=]() {
            connect(socket, &QTcpSocket::disconnected, [=]() {
                socket->deleteLater();

                createMPD();
            });
            socket->disconnectFromHost();
        });

        socket->connectToHost(m_host, m_port);
    }
}

void Controller::handleListAlbumsClick()
{
    if (!m_connection) {
        return;
    }

    for (auto album : getAlbumList()) {
        qDebug() << album;
    }
}

QVector<QString> Controller::getAlbumList()
{
    QVector<QString> albums;

    if (!m_connection) {
        return albums;
    }

    m_notifier->setEnabled(false);
    mpd_run_noidle(m_connection);

    if (!mpd_search_db_tags(m_connection, MPD_TAG_ALBUM)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return albums;
    }

    if (!mpd_search_commit(m_connection)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return albums;
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_connection, MPD_TAG_ALBUM)) != nullptr) {
        albums.push_back(pair->value);
        mpd_return_pair(m_connection, pair);
    }

    m_notifier->setEnabled(true);
    mpd_send_idle(m_connection);

    return albums;
}

QString Controller::host()
{
    return m_host;
}

void Controller::handleIdle(mpd_idle idle)
{
    qDebug() << "Controller has received an idle";
    if (!m_connection) {
        return;
    }

    if (!idle && mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        // This means we lost the connection.
        qDebug() << mpd_connection_get_error_message(m_connection);
        mpd_connection_free(m_connection);
        m_connection = nullptr;
        emit connectionState(ConnectionState::Disconnected);
    }

    if (idle & MPD_IDLE_QUEUE) {
        qDebug() << "THE QUEUE HAS CHANGED";
        emit queueChanged();
    }
}

void Controller::createMPD()
{
    auto connection = mpd_connection_new(m_host.toUtf8().constData(), m_port, m_timeout_ms);

    if (!connection) {
        // OOM
        emit unrecoverableError();
    }

    if (m_connection) {
        mpd_connection_free(m_connection);
    }

    if (m_notifier) {
        delete m_notifier;
    }

    m_connection = connection;

    if (mpd_connection_get_error(m_connection) == MPD_ERROR_SUCCESS) {
        m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_connection),
                                         QSocketNotifier::Read,
                                         this);

        connect(m_notifier, &QSocketNotifier::activated, this, &Controller::handleActivation);

        mpd_send_idle(m_connection);
        emit connectionState(ConnectionState::Connected);
    } else {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        emit connectionState(ConnectionState::Disconnected);
    }
}

void Controller::handleActivation()
{
    handleIdle(mpd_recv_idle(m_connection, false));
}

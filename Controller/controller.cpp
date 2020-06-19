#include "controller.h"
#include "connectionmanager.h"
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

    auto connectionManager = new ConnectionManager(this);
    connect(this,
            &Controller::requestConnection,
            connectionManager,
            &ConnectionManager::createConnection);
    connect(connectionManager, &ConnectionManager::mpd, this, &Controller::setMPD);
}

void Controller::handleConnectClick()
{
    emit connectionState(ConnectionState::Connecting);

    if (m_host.startsWith("/") || m_host.startsWith("@")) {
        auto socket = new QLocalSocket();

        connect(socket, &QLocalSocket::errorOccurred, [=](QLocalSocket::LocalSocketError error) {
            Q_UNUSED(error);
            emit errorMessage(socket->errorString());
            socket->deleteLater();
            emit connectionState(ConnectionState::Disconnected);
        });

        connect(socket, &QLocalSocket::connected, [=]() {
            connect(socket, &QLocalSocket::disconnected, [=]() { socket->deleteLater(); });
            socket->disconnectFromServer();
        });

        socket->connectToServer(m_host);

    } else {
        auto socket = new QTcpSocket();

        connect(socket, &QTcpSocket::errorOccurred, [=](QTcpSocket::SocketError error) {
            Q_UNUSED(error);
            emit errorMessage(socket->errorString());
            socket->deleteLater();
            emit connectionState(ConnectionState::Disconnected);
        });

        connect(socket, &QTcpSocket::connected, [=]() {
            connect(socket, &QTcpSocket::disconnected, [=]() { socket->deleteLater(); });
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

void Controller::setMPD(MPDSignalCarrier *mpd)
{
    mpd_connection *connection = mpd->connection();
    delete mpd;

    if (!connection) {
        // OOM
        emit unrecoverableError();
    }

    if (m_connection) {
        mpd_connection_free(m_connection);
    }

    m_connection = connection;

    if (mpd_connection_get_error(m_connection) == MPD_ERROR_SUCCESS) {
        qDebug() << "Creating the socket notifier";
        m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_connection),
                                         QSocketNotifier::Read,
                                         this);
        connect(m_notifier, &QSocketNotifier::activated, this, &Controller::handleActivation);
        mpd_send_idle(m_connection);
    }

    if (mpd_connection_get_error(m_connection) == MPD_ERROR_SUCCESS) {
        emit connectionState(ConnectionState::Connected);

    } else {
        qDebug() << mpd_connection_get_error_message(m_connection);
        emit connectionState(ConnectionState::Disconnected);
    }
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

void Controller::handleActivation()
{
    handleIdle(mpd_recv_idle(m_connection, false));
}

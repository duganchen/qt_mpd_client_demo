#include "controller.h"
#include "connectionmanager.h"
#include <QDebug>

Controller::Controller(QString host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
    , m_mpd(nullptr)
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
    emit requestConnection(m_host, m_port, m_timeout_ms);
}

void Controller::handleListAlbumsClick()
{
    if (!m_mpd || m_mpd->isNull()) {
        return;
    }

    for (auto album : getAlbumList()) {
        qDebug() << album;
    }
}

QVector<QString> Controller::getAlbumList()
{
    QVector<QString> albums;

    for (const char *tag : m_mpd->search_db_tags(MPD_TAG_ALBUM)) {
        albums.push_back(tag);
    }

    return albums;
}

QString Controller::host()
{
    return m_host;
}

void Controller::setMPD(MPDConnection *mpd)
{
    m_connection = mpd->connection();
    m_notifier = mpd->notifier();

    if (!mpd || mpd->isNull()) {
        // The first condition should never happens. The second means we're out of memory.
        emit unrecoverableError();
    }

    if (m_mpd) {
        delete m_mpd;
    }

    m_mpd = mpd;

    if (m_mpd->error() == MPD_ERROR_SUCCESS) {
        emit connectionState(ConnectionState::Connected);

        connect(mpd, &MPDConnection::idle, this, &Controller::handleIdle);
    } else {
        qDebug() << m_mpd->error_message();
        emit connectionState(ConnectionState::Disconnected);
    }
}

void Controller::handleIdle(mpd_idle idle)
{
    qDebug() << "Controller has received an idle";
    if (!m_mpd || m_mpd->isNull()) {
        return;
    }

    if (!idle && m_mpd->error() != MPD_ERROR_SUCCESS) {
        // This means we lost the connection.
        qDebug() << m_mpd->error_message();
        delete m_mpd;
        m_mpd = nullptr;
        emit connectionState(ConnectionState::Disconnected);
    }

    if (idle & MPD_IDLE_QUEUE) {
        qDebug() << "THE QUEUE HAS CHANGED";
        emit queueChanged();
    }
}

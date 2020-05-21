#include "controller.h"
#include <QDebug>

Controller::Controller(const char *host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
    , m_mpd(nullptr)
    , m_host(host)
    , m_port(port)
    , m_timeout_ms(timeout_ms)
{}

void Controller::handleConnectClick()
{
    emit requestConnection(m_host, m_port, m_timeout_ms);
}

void Controller::handleListAlbumsClick()
{
    if (!m_mpd || m_mpd->isNull()) {
        return;
    }

    for (const char *tag : m_mpd->search_db_tags(MPD_TAG_ALBUM)) {
        qDebug() << tag;
    }
}

void Controller::setMPD(MPDConnection *mpd)
{
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
}

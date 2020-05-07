#include "controller.h"
#include "mockmpdsettings.h"
#include <QDebug>

Controller::Controller(AbstractMPDSettings *mpd_settings, QObject *parent)
    : QObject(parent), m_mpd(nullptr), m_mpd_settings(mpd_settings)
{
}

void Controller::handleConnectClick()
{
    emit requestConnection(m_mpd_settings);
}

void Controller::handleListAlbumsClick()
{
    if (!m_mpd || m_mpd->isNull())
    {
        return;
    }

    for (const char *tag : m_mpd->search_db_tags(MPD_TAG_ALBUM))
    {
        qDebug() << tag;
    }
}

void Controller::setMPD(AbstractMPDConnection *mpd)
{
    if (!mpd || mpd->isNull())
    {
        // The first condition should never happens. The second means we're out of memory.
        emit unrecoverableError();
    }

    if (m_mpd)
    {
        delete m_mpd;
    }

    m_mpd = mpd;

    if (m_mpd->error() == MPD_ERROR_SUCCESS)
    {
        emit connectionState(ConnectionState::Connected);

        connect(mpd, &AbstractMPDConnection::idle, this, &Controller::handleIdle);
    }
    else
    {
        qDebug() << m_mpd->error_message();
        emit connectionState(ConnectionState::Disconnected);
    }
}

void Controller::handleIdle(mpd_idle idle)
{
    qDebug() << "Controller has received an idle";
    if (!m_mpd || m_mpd->isNull())
    {
        return;
    }

    if (!idle && m_mpd->error() != MPD_ERROR_SUCCESS)
    {
        // This means we lost the connection.
        qDebug() << m_mpd->error_message();
        delete m_mpd;
        m_mpd = nullptr;
        emit connectionState(ConnectionState::Disconnected);
    }
}

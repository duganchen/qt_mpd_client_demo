#include "mpdconnection.h"
#include <QDebug>
#include <QSocketNotifier>
#include <mpd/client.h>

MPDConnection::MPDConnection(AbstractMPDSettings *settings, QObject *parent)
    : AbstractMPDConnection(settings, parent), m_notifier(nullptr)
{
    // Note: This DOES block long enough to become a problem!

    qDebug() << "Host is " << settings->host();
    qDebug() << "Port is " << settings->port();
    qDebug() << "Timeout is " << settings->timeout_ms();

    m_mpd = mpd_connection_new(settings->host(), settings->port(), settings->timeout_ms());

    qDebug() << "mpd_connection_new is done";

    if (mpd_connection_get_error(m_mpd) == MPD_ERROR_SUCCESS)
    {
        qDebug() << "Creating the socket notifier";
        m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_mpd), QSocketNotifier::Read, this);
        connect(m_notifier, &QSocketNotifier::activated, this, &MPDConnection::handleActivation);
        mpd_send_idle(m_mpd);
    }
}

MPDConnection::~MPDConnection()
{
    qDebug() << "Deleting the connection";
    if (m_mpd)
    {
        mpd_connection_free(m_mpd);
    }
}

mpd_error MPDConnection::error()
{
    return mpd_connection_get_error(m_mpd);
}

const char *MPDConnection::error_message()
{
    return mpd_connection_get_error_message(m_mpd);
}

bool MPDConnection::isNull() const
{
    return nullptr == m_mpd;
}

int MPDConnection::fd()
{
    return mpd_connection_get_fd(m_mpd);
}

bool MPDConnection::send_idle()
{
    return mpd_send_idle(m_mpd);
}

mpd_idle MPDConnection::run_noidle()
{
    return mpd_run_noidle(m_mpd);
}

QVector<const char *> MPDConnection::search_db_tags(mpd_tag_type type)
{
    QVector<const char *> tags;
    if (!m_mpd || !m_notifier)
    {
        return tags;
    }

    m_notifier->setEnabled(false);
    emit idle(mpd_run_noidle(m_mpd));

    if (!mpd_search_db_tags(m_mpd, type))
    {
        qDebug() << mpd_connection_get_error_message(m_mpd);
        return tags;
    }

    if (!mpd_search_commit(m_mpd))
    {
        qDebug() << mpd_connection_get_error_message(m_mpd);
        return tags;
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_mpd, type)) != nullptr)
    {
        tags.push_back(pair->value);
        mpd_return_pair(m_mpd, pair);
    }

    m_notifier->setEnabled(true);
    mpd_send_idle(m_mpd);

    return tags;
}

void MPDConnection::handleActivation()
{
    emit idle(mpd_recv_idle(m_mpd, false));
}

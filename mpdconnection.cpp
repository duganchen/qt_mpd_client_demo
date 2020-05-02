#include "mpdconnection.h"

MPDConnection::MPDConnection(const char *host, unsigned port, unsigned timeout_ms, QObject *parent): QObject(parent), m_mpd(nullptr), m_notifier(nullptr)
{
    m_mpd = mpd_connection_new(host, port, timeout_ms);

    if (m_mpd == nullptr)
    {
        return;
    }

    if (MPD_ERROR_SUCCESS != mpd_connection_get_error(m_mpd))
    {
        return;
    }

    m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_mpd), QSocketNotifier::Read, this);

    connect(m_notifier, &QSocketNotifier::activated, [=]{emit activated();});
}


bool MPDConnection::isNull()
{
    // Use this check that the constructor didn't fail due to OOM
    return m_mpd == nullptr;
}


mpd_error MPDConnection::error()
{
    return mpd_connection_get_error(m_mpd);
}

QString MPDConnection::errorMessage()
{
    return mpd_connection_get_error_message(m_mpd);
}


bool MPDConnection::searchDBTags(enum mpd_tag_type type)
{
    return mpd_search_db_tags(m_mpd, type);
}

bool MPDConnection::searchCommit()
{
    return mpd_search_commit(m_mpd);
}

QVector<QPair<QString, QString>> MPDConnection::recvPairTags(enum mpd_tag_type type)
{
    QVector<QPair<QString, QString>> pairs;
    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_mpd, type)) != nullptr)
    {
        pairs.push_back(QPair<QString, QString>(pair->name, pair->value));
        mpd_return_pair(m_mpd, pair);
    }
    return pairs;
}

bool MPDConnection::sendIdle()
{
    return mpd_send_idle(m_mpd);
}

enum mpd_idle MPDConnection::recvIdle(bool disable_timeout)
{
    return mpd_recv_idle(m_mpd, disable_timeout);
}

QString MPDConnection::idleName(enum mpd_idle idle)
{
    return mpd_idle_name(idle);
}

mpd_idle MPDConnection::runNoIdle()
{
    return mpd_run_noidle(m_mpd);
}

MPDConnection::~MPDConnection()
{
    if (m_mpd != nullptr)
    {
        mpd_connection_free(m_mpd);
    }
}

void MPDConnection::setNotifierEnabled(bool enabled)
{
    m_notifier->setEnabled(enabled);
}

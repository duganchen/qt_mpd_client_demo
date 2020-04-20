#include "mpdconnection.h"

MPDConnection::MPDConnection(const char *host, unsigned port, unsigned timeout_ms, QObject *parent): QObject(parent), m_commander(nullptr), m_idler(nullptr), m_notifier(nullptr)
{
    m_idler = mpd_connection_new(host, port, timeout_ms);

    if (m_idler == nullptr)
    {
        return;
    }

    if (MPD_ERROR_SUCCESS != mpd_connection_get_error(m_idler))
    {
        return;
    }

    m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_idler), QSocketNotifier::Read, this);

    m_commander = mpd_connection_new(host, port, timeout_ms);

    connect(m_notifier, &QSocketNotifier::activated, [=]{emit activated();});
}


bool MPDConnection::isNull()
{
    // Use this check that the constructor didn't fail due to OOM
    return m_idler == nullptr;
}


mpd_error MPDConnection::error()
{
    return mpd_connection_get_error(m_idler);
}

QString MPDConnection::errorMessage()
{
    return mpd_connection_get_error_message(m_idler);
}


bool MPDConnection::searchDBTags(enum mpd_tag_type type)
{
    return mpd_search_db_tags(m_commander, type);
}

bool MPDConnection::searchCommit()
{
    return mpd_search_commit(m_commander);
}

QVector<QPair<QString, QString>> MPDConnection::recvPairTags(enum mpd_tag_type type)
{
    QVector<QPair<QString, QString>> pairs;
    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_commander, type)) != nullptr)
    {
        pairs.push_back(QPair<QString, QString>(pair->name, pair->value));
        mpd_return_pair(m_commander, pair);
    }
    return pairs;
}

bool MPDConnection::sendIdle()
{
    return mpd_send_idle(m_idler);
}

enum mpd_idle MPDConnection::recvIdle(bool disable_timeout)
{
    return mpd_recv_idle(m_idler, disable_timeout);
}

QString MPDConnection::idleName(enum mpd_idle idle)
{
    return mpd_idle_name(idle);
}

MPDConnection::~MPDConnection()
{
    if (m_idler != nullptr)
    {
        mpd_connection_free(m_idler);
    }
    if (m_commander != nullptr)
    {
        mpd_connection_free(m_commander);
    }
}

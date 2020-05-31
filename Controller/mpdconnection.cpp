#include "mpdconnection.h"
#include <mpd/client.h>
#include <QDebug>
#include <QSocketNotifier>

MPDConnection::MPDConnection(QString host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
{
    // Note: This DOES block long enough to become a problem!
    m_mpd = mpd_connection_new(host.toUtf8().constData(), port, timeout_ms);
}

mpd_connection *MPDConnection::connection()
{
    return m_mpd;
}

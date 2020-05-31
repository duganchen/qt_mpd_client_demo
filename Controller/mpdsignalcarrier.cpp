#include "mpdsignalcarrier.h"
#include <mpd/client.h>
#include <QDebug>

MPDSignalCarrier::MPDSignalCarrier(QString host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
{
    // Note: This DOES block long enough to become a problem!
    m_mpd = mpd_connection_new(host.toUtf8().constData(), port, timeout_ms);
}

mpd_connection *MPDSignalCarrier::connection()
{
    return m_mpd;
}

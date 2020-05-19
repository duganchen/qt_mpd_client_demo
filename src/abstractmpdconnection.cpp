#include "abstractmpdconnection.h"

AbstractMPDConnection::AbstractMPDConnection(const char *host, unsigned port, unsigned timeout_ms, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(host)
    Q_UNUSED(port)
    Q_UNUSED(timeout_ms)
}

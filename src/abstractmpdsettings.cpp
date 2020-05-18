#include "abstractmpdsettings.h"

AbstractMPDSettings::AbstractMPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                                         const char *password, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(host)
    Q_UNUSED(port)
    Q_UNUSED(timeout_ms)
    Q_UNUSED(reserved)
    Q_UNUSED(password)
}

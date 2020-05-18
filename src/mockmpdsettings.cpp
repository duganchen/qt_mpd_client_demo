#include "mockmpdsettings.h"

MockMPDSettings::MockMPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                                 const char *password, QObject *parent)
    : AbstractMPDSettings(host, port, timeout_ms, reserved, password, parent), m_host(host), m_port(port),
      m_timeout_ms(timeout_ms), m_reserved(reserved), m_password(password)
{
}

const char *MockMPDSettings::host()
{
    return m_host;
}

unsigned MockMPDSettings::port()
{
    return m_port;
}

unsigned MockMPDSettings::timeout_ms()
{
    return m_timeout_ms;
}

const char *MockMPDSettings::password()
{
    return m_password;
}

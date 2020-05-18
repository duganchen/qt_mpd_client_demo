#ifndef MOCKMPDSETTINGS_H
#define MOCKMPDSETTINGS_H

#include "abstractmpdsettings.h"

class MockMPDSettings : public AbstractMPDSettings
{
    Q_OBJECT
public:
    explicit MockMPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                             const char *password, QObject *parent = nullptr);
    virtual const char *host();
    virtual unsigned port();
    virtual unsigned timeout_ms();
    virtual const char *password();

private:
    const char *m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
    const char *m_reserved;
    const char *m_password;
};

#endif // MOCKMPDSETTINGS_H

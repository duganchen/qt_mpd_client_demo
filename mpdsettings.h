#ifndef MPDSETTINGS_H
#define MPDSETTINGS_H

#include "abstractmpdsettings.h"

class MPDSettings : public AbstractMPDSettings
{
public:
    explicit MPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                         const char *password, QObject *parent = nullptr);

    virtual const char *host();
    virtual unsigned port();
    virtual unsigned timeout_ms();
    virtual const char *password();

    ~MPDSettings();

private:
    struct mpd_settings *m_mpd_settings;
};

#endif // MPDSETTINGS_H

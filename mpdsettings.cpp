#include "mpdsettings.h"
#include <mpd/client.h>

MPDSettings::MPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                         const char *password, QObject *parent)
    : AbstractMPDSettings(host, port, timeout_ms, reserved, password, parent)
{
    m_mpd_settings = mpd_settings_new(host, port, timeout_ms, reserved, password);
}

const char *MPDSettings::host()
{
    return mpd_settings_get_host(m_mpd_settings);
}

unsigned MPDSettings::port()
{
    return mpd_settings_get_port(m_mpd_settings);
}

unsigned MPDSettings::timeout_ms()
{
    return mpd_settings_get_timeout_ms(m_mpd_settings);
}

const char *MPDSettings::password()
{
    return mpd_settings_get_password(m_mpd_settings);
}

MPDSettings::~MPDSettings()
{
    mpd_settings_free(m_mpd_settings);
}

#ifndef ABSTRACTMPDSETTINGS_H
#define ABSTRACTMPDSETTINGS_H

#include <QObject>

class AbstractMPDSettings : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMPDSettings(const char *host, unsigned port, unsigned timeout_ms, const char *reserved,
                                 const char *password, QObject *parent = nullptr);

    virtual const char *host() = 0;
    virtual unsigned port() = 0;
    virtual unsigned timeout_ms() = 0;
    virtual const char *password() = 0;
};

#endif // ABSTRACTMPDSETTINGS_H

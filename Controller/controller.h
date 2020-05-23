#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "connectionstate.h"
#include "mpdconnection.h"
#include <QObject>

#include "Controller_global.h"

class CONTROLLER_EXPORT Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const char *, unsigned, unsigned, QObject *parent = nullptr);
public slots:
    void handleConnectClick();
    void handleListAlbumsClick();
    void setMPD(MPDConnection *);
signals:
    void errorMessage(QString);
    void connectionState(ConnectionState);

    void requestConnection(const char *, unsigned, unsigned);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

private:
    void handleIdle(mpd_idle);

    MPDConnection *m_mpd;
    const char *m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
};

#endif // CONTROLLER_H

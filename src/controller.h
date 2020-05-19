#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "abstractmpdconnection.h"
#include "connectionstate.h"
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const char *, unsigned, unsigned, QObject *parent = nullptr);
public slots:
    void handleConnectClick();
    void handleListAlbumsClick();
    void setMPD(AbstractMPDConnection *);
signals:
    void errorMessage(QString);
    void connectionState(ConnectionState);

    void requestConnection(const char *, unsigned, unsigned);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

private:
    void handleIdle(mpd_idle);

    AbstractMPDConnection *m_mpd;
    const char *m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
};

#endif // CONTROLLER_H

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mpdconnection.h"
#include <QObject>

#include "Controller_global.h"

class CONTROLLER_EXPORT Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QString, unsigned, unsigned, QObject *parent = nullptr);

    enum class ConnectionState { Disconnected, Connecting, Connected };
    Q_ENUM(ConnectionState)
public slots:
    void handleConnectClick();
    void handleListAlbumsClick();
signals:
    void errorMessage(QString);
    void connectionState(Controller::ConnectionState connectionState);

    void requestConnection(QString, unsigned, unsigned);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

private:
    void handleIdle(mpd_idle);

    MPDConnection *m_mpd;
    QString m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
private slots:
    void setMPD(MPDConnection *);
};

Q_DECLARE_METATYPE(Controller::ConnectionState);

#endif // CONTROLLER_H

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "abstractmpdconnection.h"
#include "abstractmpdsettings.h"
#include "connectionstate.h"
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(AbstractMPDSettings *, QObject *parent = nullptr);
public slots:
    void handleConnectClick();
    void handleListAlbumsClick();
    void setMPD(AbstractMPDConnection *);
signals:
    void errorMessage(QString);
    void connectionState(ConnectionState);

    void requestConnection(AbstractMPDSettings *);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

private:
    void handleIdle(mpd_idle);

    AbstractMPDConnection *m_mpd;
    AbstractMPDSettings *m_mpd_settings;
};

#endif // CONTROLLER_H

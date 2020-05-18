#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "abstractmpdconnection.h"
#include "abstractmpdsettings.h"
#include "mpdconnectionfactory.h"
#include <QObject>
#include <QThread>

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();
public slots:
    void createConnection(AbstractMPDSettings *);
    void setMPD(AbstractMPDConnection *);
signals:
    void mpd(AbstractMPDConnection *);
    void requestConnectionFromFactory(AbstractMPDSettings *);

private:
    QThread connectionThread;
};

#endif // CONNECTIONMANAGER_H

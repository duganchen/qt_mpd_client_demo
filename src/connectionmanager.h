#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "abstractmpdconnection.h"
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
    void createConnection(const char *, unsigned, unsigned);
    void setMPD(AbstractMPDConnection *);
signals:
    void mpd(AbstractMPDConnection *);
    void requestConnectionFromFactory(const char *, unsigned, unsigned);

private:
    QThread connectionThread;
};

#endif // CONNECTIONMANAGER_H

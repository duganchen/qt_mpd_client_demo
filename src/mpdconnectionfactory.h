#ifndef MPDCONNECTIONFACTORY_H
#define MPDCONNECTIONFACTORY_H

#include "abstractmpdsettings.h"
#include "mpdconnection.h"
#include <QObject>

class MPDConnectionFactory : public QObject
{
    Q_OBJECT
public:
    explicit MPDConnectionFactory(QObject *parent = nullptr);
public slots:
    void createConnection(AbstractMPDSettings *);
signals:
    void mpd(MPDConnection *);
};

#endif // MPDCONNECTIONFACTORY_H

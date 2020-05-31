#ifndef MPDCONNECTIONFACTORY_H
#define MPDCONNECTIONFACTORY_H

#include "mpdsignalcarrier.h"
#include <QObject>

class MPDConnectionFactory : public QObject
{
    Q_OBJECT
public:
    explicit MPDConnectionFactory(QObject *parent = nullptr);
public slots:
    void createConnection(QString, unsigned, unsigned);
signals:
    void mpd(MPDSignalCarrier *);
};

#endif // MPDCONNECTIONFACTORY_H

#ifndef MPDCONNECTOR_H
#define MPDCONNECTOR_H

#include "mpdconnection.h"
#include <QObject>

class MPDConnector : public QObject
{
    Q_OBJECT
public:
    explicit MPDConnector(QObject *parent = nullptr);
public slots:
    void createConnection();

signals:
    void mpdConnection(MPDConnection *);
};

#endif // MPDCONNECTOR_H

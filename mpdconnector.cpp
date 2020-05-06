#include "mpdconnector.h"
#include "mpdconnection.h"
#include <QDebug>

MPDConnector::MPDConnector(QObject *parent) : QObject(parent)
{
}

void MPDConnector::createConnection()
{
    qDebug() << "CREATING CONNECTION";
    emit mpdConnection(new MPDConnection());
}

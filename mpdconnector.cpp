#include "mpdconnector.h"
#include "mpdconnection.h"
#include <QApplication>
#include <QDebug>

MPDConnector::MPDConnector(QObject *parent) : QObject(parent)
{
}

void MPDConnector::createConnection()
{
    qDebug() << "CREATING CONNECTION";

    auto conn = new MPDConnection();
    conn->moveToThread(QApplication::instance()->thread());
    emit mpdConnection(conn);
}

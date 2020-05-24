#include "mpdconnectionfactory.h"
#include "mpdconnection.h"
#include <QApplication>
#include <QDebug>

MPDConnectionFactory::MPDConnectionFactory(QObject *parent)
    : QObject(parent)
{}

void MPDConnectionFactory::createConnection(QString host, unsigned port, unsigned timeout_ms)
{
    qDebug() << "Factory is creating connection";
    qDebug() << "createConnection(" << host << ", " << port << ", " << timeout_ms << ")";

    auto mpdConnection = new MPDConnection(host, port, timeout_ms);
    mpdConnection->setParent(nullptr);
    mpdConnection->moveToThread(QApplication::instance()->thread());
    emit mpd(mpdConnection);
}

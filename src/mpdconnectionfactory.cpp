#include "mpdconnectionfactory.h"
#include "mpdconnection.h"
#include <QApplication>
#include <QDebug>

MPDConnectionFactory::MPDConnectionFactory(QObject *parent) : QObject(parent)
{
}

void MPDConnectionFactory::createConnection(AbstractMPDSettings *settings)
{
    qDebug() << "Factory is creating connection";

    auto mpdConnection = new MPDConnection(settings);
    mpdConnection->setParent(nullptr);
    mpdConnection->moveToThread(QApplication::instance()->thread());
    emit mpd(mpdConnection);
}

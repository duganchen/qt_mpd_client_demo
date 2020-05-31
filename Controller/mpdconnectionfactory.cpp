#include "mpdconnectionfactory.h"
#include "mpdsignalcarrier.h"
#include <QApplication>
#include <QDebug>

MPDConnectionFactory::MPDConnectionFactory(QObject *parent)
    : QObject(parent)
{}

void MPDConnectionFactory::createConnection(QString host, unsigned port, unsigned timeout_ms)
{
    auto mpdConnection = new MPDSignalCarrier(host, port, timeout_ms);
    mpdConnection->setParent(nullptr);
    mpdConnection->moveToThread(QApplication::instance()->thread());
    emit mpd(mpdConnection);
}

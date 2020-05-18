#include "connectionmanager.h"
#include "mpdconnectionfactory.h"
#include <QDebug>

ConnectionManager::ConnectionManager(QObject *parent) : QObject(parent)
{
    auto connectionFactory = new MPDConnectionFactory();
    connectionFactory->moveToThread(&connectionThread);

    connect(&connectionThread, &QThread::finished, connectionFactory, &MPDConnectionFactory::deleteLater);
    connect(this, &ConnectionManager::requestConnectionFromFactory, connectionFactory,
            &MPDConnectionFactory::createConnection);
    connect(connectionFactory, &MPDConnectionFactory::mpd, this, &ConnectionManager::setMPD);

    connectionThread.start();
}

ConnectionManager::~ConnectionManager()
{
    connectionThread.quit();
    connectionThread.wait();
}

void ConnectionManager::createConnection(AbstractMPDSettings *settings)
{
    qDebug() << "Requesting connection from factory";
    emit requestConnectionFromFactory(settings);
}

void ConnectionManager::setMPD(AbstractMPDConnection *conn)
{
    qDebug() << "Connection manager has received an instance";
    emit mpd(conn);
}

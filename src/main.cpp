#include "connectionmanager.h"
#include "controller.h"
#include "mainwindow.h"
#include "mockmpdsettings.h"
#include <QApplication>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MockMPDSettings mpd_settings("localhost", 6600, 200, nullptr, nullptr);
    Controller controller(&mpd_settings);
    ConnectionManager connectionManager;
    QObject::connect(&w, &MainWindow::listAlbumsClicked, &controller, &Controller::handleListAlbumsClick);
    QObject::connect(&w, &MainWindow::connectClicked, &controller, &Controller::handleConnectClick);
    QObject::connect(&controller, &Controller::connectionState, &w, &MainWindow::setConnectionState);
    QObject::connect(&controller, &Controller::requestConnection, &connectionManager,
                     &ConnectionManager::createConnection);
    QObject::connect(&connectionManager, &ConnectionManager::mpd, &controller, &Controller::setMPD);

    QObject::connect(&controller, &Controller::unrecoverableError, &a, &QApplication::quit);

    w.show();
    return a.exec();
}

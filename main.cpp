#include "mainwindow.h"

#include <mpd/client.h>

#include <QApplication>
#include <iostream>
#include <QDebug>

#include <QSocketNotifier>

int main(int argc, char *argv[])
{
    // Note: qDebug prints nothing by default in Fedora.
    // Hence this sanityc checkj.
    // See: https://stackoverflow.com/q/12799653/240515
    qDebug() << "Do you see this?";

    auto idle_connection = mpd_connection_new("localhost", 6600, 0);
    if (mpd_connection_get_error(idle_connection) != MPD_ERROR_SUCCESS)
    {
        qDebug() << mpd_connection_get_error_message(idle_connection);
        return 1;
    }
    QSharedPointer<mpd_connection> idler(idle_connection, mpd_connection_free);

    auto command_connection = mpd_connection_new("localhost", 6600, 0);
    if (mpd_connection_get_error(command_connection) != MPD_ERROR_SUCCESS)
    {
        qDebug() << mpd_connection_get_error_message(command_connection);
        return 1;
    }
    QSharedPointer<mpd_connection> commander(command_connection, mpd_connection_free);

    QApplication a(argc, argv);

    MainWindow w(commander, idler);

    w.show();
    return a.exec();
}

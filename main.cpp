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

    QApplication a(argc, argv);

    MainWindow w("localhost", 6600, 0);

    w.show();
    return a.exec();
}

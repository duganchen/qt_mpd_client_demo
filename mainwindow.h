#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSharedPointer>
#include <QSocketNotifier>
#include <mpd/client.h>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSharedPointer<mpd_connection> commander, QSharedPointer<mpd_connection> idler, QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H

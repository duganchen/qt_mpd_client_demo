#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSharedPointer>
#include <QSocketNotifier>
#include <mpd/client.h>
#include <QMainWindow>
#include "mpdconnection.h"
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const char *host, unsigned port, unsigned timeout_ms, QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void listAlbums();
    void recvNotification();
    void handleNotification(mpd_idle);
private:
    const char *m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
    MPDConnection *m_mpd;
    QPushButton *albumListButton;
};
#endif // MAINWINDOW_H

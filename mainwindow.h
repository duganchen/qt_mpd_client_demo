#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mpdconnection.h"
#include <QMainWindow>
#include <QPushButton>
#include <QSharedPointer>
#include <QSocketNotifier>
#include <QThread>
#include <mpd/client.h>

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
    void setConnection(MPDConnection *);

signals:
    void startConnecting();

private:
    const char *m_host;
    unsigned m_port;
    unsigned m_timeout_ms;
    MPDConnection *m_mpd;
    QPushButton *albumListButton;
    QThread connectionThread;
};
#endif // MAINWINDOW_H

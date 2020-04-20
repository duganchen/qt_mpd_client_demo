#include <mpd/client.h>
#include <QDebug>
#include <QPushButton>
#include "mainwindow.h"

MainWindow::MainWindow(QSharedPointer<mpd_connection> commander, QSharedPointer<mpd_connection> idler, QWidget *parent)
    : QMainWindow(parent)
{
    mpd_send_idle(idler.data());
    auto notifier = new QSocketNotifier(mpd_connection_get_fd(idler.data()), QSocketNotifier::Read, this);

    connect(notifier, &QSocketNotifier::activated, [=](){
        auto idle = mpd_recv_idle(idler.data(), false);
        if (idle != 0)
        {
            qDebug() << mpd_idle_name(idle);
            mpd_send_idle(idler.data());
        }
        else
        {
            if (mpd_connection_get_error(idler.data()) != MPD_ERROR_SUCCESS)
            {
                qDebug() << mpd_connection_get_error_message(idler.data());
                delete notifier;
            }
        }

    });

    auto button = new QPushButton("&List Albums", this);
    connect(button, &QPushButton::clicked, [=](){
        mpd_search_db_tags(commander.data(), MPD_TAG_ALBUM);
        mpd_search_commit(commander.data());

        struct mpd_pair *pair;
        while ((pair = mpd_recv_pair_tag(commander.data(), MPD_TAG_ALBUM)) != nullptr)
        {
            qDebug() << pair->value;
            mpd_return_pair(commander.data(), pair);
        }
    });
    setCentralWidget(button);
}

MainWindow::~MainWindow()
{
}


#include "mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QtNetwork/QHostInfo>
#include <mpd/client.h>

MainWindow::MainWindow(const char *host, unsigned port, unsigned timeout_ms, QWidget *parent)
    : QMainWindow(parent), m_host(host), m_port(port), m_timeout_ms(timeout_ms), m_mpd(nullptr),
      albumListButton(nullptr)
{
    albumListButton = new QPushButton("&List Albums", this);
    connect(albumListButton, &QPushButton::clicked, this, &MainWindow::listAlbums);
    setCentralWidget(albumListButton);

    setEnabled(false);

    QHostInfo::lookupHost(m_host, this, [=](QHostInfo info) {
        if (info.error() != QHostInfo::NoError)
        {
            qDebug() << info.errorString();
            return;
        }

        m_mpd = new MPDConnection(this);
        m_mpd->connectToMPD();
        if (m_mpd->isNull())
        {
            qDebug() << "Cannot allocate MPD connection.";
            return;
        }

        if (m_mpd->error() != MPD_ERROR_SUCCESS)
        {
            qDebug() << m_mpd->errorMessage();
            return;
        }

        setEnabled(true);

        connect(m_mpd, &MPDConnection::activated, this, &MainWindow::recvNotification);

        m_mpd->sendIdle();
    });
}

void MainWindow::recvNotification()
{
    qDebug() << "RECEIVING NOTIFICATION";
    auto idle = m_mpd->recvIdle(false);
    handleNotification(idle);
}

void MainWindow::handleNotification(mpd_idle idle)
{
    if (idle != 0)
    {
        if (idle & MPD_IDLE_DATABASE)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_DATABASE);
        }

        if (idle & MPD_IDLE_STORED_PLAYLIST)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_STORED_PLAYLIST);
        }

        if (idle & MPD_IDLE_QUEUE)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_QUEUE);
        }

        if (idle & MPD_IDLE_PLAYER)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_PLAYER);
        }

        if (idle & MPD_IDLE_MIXER)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_MIXER);
        }

        if (idle & MPD_IDLE_OUTPUT)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_OUTPUT);
        }

        if (idle & MPD_IDLE_OPTIONS)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_OPTIONS);
        }

        if (idle & MPD_IDLE_UPDATE)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_UPDATE);
        }

        if (idle & MPD_IDLE_STICKER)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_STICKER);
        }

        if (idle & MPD_IDLE_SUBSCRIPTION)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_SUBSCRIPTION);
        }

        if (idle & MPD_IDLE_MESSAGE)
        {
            qDebug() << mpd_idle_name(MPD_IDLE_MESSAGE);
        }

        m_mpd->sendIdle();
    }
    else if (m_mpd->error() != MPD_ERROR_SUCCESS)
    {
        qDebug() << m_mpd->errorMessage();
        setEnabled(false);

        m_mpd->deleteLater();
    }
}

void MainWindow::listAlbums()
{
    m_mpd->setNotifierEnabled(false);
    handleNotification(m_mpd->runNoIdle());
    m_mpd->searchDBTags(MPD_TAG_ALBUM);
    m_mpd->searchCommit();
    for (auto pair : m_mpd->recvPairTags(MPD_TAG_ALBUM))
    {
        qDebug() << pair.second;
    }
    m_mpd->setNotifierEnabled(true);
    m_mpd->sendIdle();
}

MainWindow::~MainWindow()
{
}

#include <mpd/client.h>
#include <QDebug>
#include <QPushButton>
#include <QtNetwork/QHostInfo>
#include "mainwindow.h"

MainWindow::MainWindow(const char *host, unsigned port, unsigned timeout_ms, QWidget *parent)
    : QMainWindow(parent), m_host(host), m_port(port), m_timeout_ms(timeout_ms), m_mpd(nullptr), albumListButton(nullptr)
{
    albumListButton = new QPushButton("&List Albums", this);
    connect(albumListButton, &QPushButton::clicked, this, &MainWindow::listAlbums);
    setCentralWidget(albumListButton);

    setEnabled(false);

    QHostInfo::lookupHost(m_host, this, [=](QHostInfo info){
        if (info.error() != QHostInfo::NoError)
        {
            qDebug() << info.errorString();
            return;
        }

        m_mpd = new MPDConnection(m_host, m_port, m_timeout_ms, this);
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
    auto idle = m_mpd->recvIdle(false);
    if (idle != 0)
    {
        qDebug() << MPDConnection::idleName(idle);
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
    m_mpd->searchDBTags(MPD_TAG_ALBUM);
    m_mpd->searchCommit();
    for (auto pair: m_mpd->recvPairTags(MPD_TAG_ALBUM))
    {
        qDebug() << pair.second;
    }
}


MainWindow::~MainWindow()
{
}


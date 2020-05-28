#include "mpdprocess.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

MPDProcess::MPDProcess(QObject *parent)
    : QObject(parent)
{
    m_mpdProc = new QProcess(this);

    QFile templateFile{"resources/mpd.conf"};
    templateFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in{&templateFile};
    QString tmplate{in.readAll()};
    tmplate = tmplate.arg(QCoreApplication::applicationDirPath() + "/resources/Music");
    QDir().mkdir(m_temp.path() + "/playlists");
    tmplate = tmplate.arg(m_temp.path());

    auto confPath = m_temp.path() + "/mpd.conf";
    QFile confFile{confPath};
    confFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream confStream(&confFile);
    confStream << tmplate;
    confFile.close();

    // For now, we don't need to copy the audio files to the temporary directory. The tests
    // don't modify them. If that changes, see:
    // https://forum.qt.io/topic/105993/copy-folder-qt-c

    QStringList args{"-v", "--no-daemon", confPath};

    m_mpdProc = new QProcess(this);

    // On OS X, you may need to go to Projects->Build and add
    // /usr/local/bin to the PATH in the build environment, if that's
    // where mpd is installed.
    m_mpdProc->start("mpd", args);
}

MPDProcess::~MPDProcess()
{
    auto confPath = m_temp.path() + "/mpd.conf";
    QStringList args{"--kill", confPath};
    QProcess killer;
    killer.start("mpd", args);
    killer.waitForFinished();
}

#ifndef MPDPROCESS_H
#define MPDPROCESS_H

#include <QObject>
#include <QProcess>
#include <QTemporaryDir>

// There's a delay between the time that you start MPD and when it
// starts accepting connections.This wait-time works reliably on my
// 2015 MacBook Pro.
constexpr int MPD_START_MS{500};

class MPDProcess : public QObject
{
    Q_OBJECT
public:
    explicit MPDProcess(QObject *parent = nullptr);
    virtual ~MPDProcess();

    QString socketPath();

private:
    QTemporaryDir m_temp;
    QProcess *m_mpdProc;
};

#endif // MPDPROCESS_H

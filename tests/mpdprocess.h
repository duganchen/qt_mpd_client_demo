#ifndef MPDPROCESS_H
#define MPDPROCESS_H

#include <QObject>
#include <QProcess>
#include <QTemporaryDir>

class MPDProcess : public QObject
{
    Q_OBJECT
public:
    explicit MPDProcess(QObject *parent = nullptr);
    virtual ~MPDProcess();

private:
    QTemporaryDir m_temp;
    QProcess *m_mpdProc;
};

#endif // MPDPROCESS_H

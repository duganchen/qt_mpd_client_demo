#ifndef MPDCONNECTION_H
#define MPDCONNECTION_H

#include <QObject>
#include <QSocketNotifier>
#include <QVector>
#include <mpd/client.h>

class MPDConnection : public QObject
{
    Q_OBJECT
public:
    MPDConnection(QObject *parent = nullptr);
    ~MPDConnection();

    mpd_error error();
    QString errorMessage();
    bool isNull();
    bool searchDBTags(enum mpd_tag_type);
    bool searchCommit();
    QVector<QPair<QString, QString>> recvPairTags(enum mpd_tag_type);
    bool sendIdle();
    enum mpd_idle recvIdle(bool);
    static QString idleName(enum mpd_idle);
    mpd_idle runNoIdle();

    void setNotifierEnabled(bool);
signals:
    void activated();

private:
    struct mpd_connection *m_mpd;
    QSocketNotifier *m_notifier;
};

#endif // MPDCONNECTION_H

#ifndef MPDCONNECTION_H
#define MPDCONNECTION_H

#include <mpd/client.h>
#include <QObject>
#include <QSocketNotifier>

class MPDConnection : public QObject
{
    Q_OBJECT
public:
    explicit MPDConnection(QString, unsigned, unsigned, QObject *parent = nullptr);
#if 0
    ~MPDConnection();

    mpd_error error();
    const char *error_message();
    bool isNull() const;
    int fd();
    bool send_idle();
    mpd_idle run_noidle();
    QVector<const char *> search_db_tags(mpd_tag_type);
#endif
    mpd_connection *connection();
#if 0
    QSocketNotifier *notifier();

signals:
    void idle(mpd_idle);
#endif
private:
    mpd_connection *m_mpd;
#if 0
    QSocketNotifier *m_notifier;
private slots:
    void handleActivation();
#endif
};

#endif // MPDCONNECTION_H

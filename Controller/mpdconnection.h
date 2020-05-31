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
    ~MPDConnection();

    mpd_error error();
    const char *error_message();
    bool isNull() const;
    int fd();
    bool send_idle();
    mpd_idle run_noidle();
    QVector<const char *> search_db_tags(mpd_tag_type);

    mpd_connection *connection();

signals:
    void idle(mpd_idle);

private:
    mpd_connection *m_mpd;
    QSocketNotifier *m_notifier;
private slots:
    void handleActivation();
};

#endif // MPDCONNECTION_H

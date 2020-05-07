#ifndef MPDCONNECTION_H
#define MPDCONNECTION_H

#include "abstractmpdconnection.h"
#include "abstractmpdsettings.h"
#include <QObject>
#include <QSocketNotifier>
#include <mpd/client.h>

class MPDConnection : public AbstractMPDConnection
{
    Q_OBJECT
public:
    explicit MPDConnection(AbstractMPDSettings *, QObject *parent = nullptr);
    ~MPDConnection();

    virtual mpd_error error();
    virtual const char *error_message();
    virtual bool isNull() const;
    virtual int fd();
    virtual bool send_idle();
    virtual mpd_idle run_noidle();
    virtual QVector<const char *> search_db_tags(mpd_tag_type);

private:
    mpd_connection *m_mpd;
    QSocketNotifier *m_notifier;
private slots:
    void handleActivation();
};

#endif // MPDCONNECTION_H

#ifndef ABSTRACTMPDCONNECTION_H
#define ABSTRACTMPDCONNECTION_H

#include <QObject>
#include <mpd/client.h>

class AbstractMPDConnection : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMPDConnection(const char *, unsigned, unsigned, QObject *parent = nullptr);

    virtual mpd_error error() = 0;
    virtual const char *error_message() = 0;
    virtual bool isNull() const = 0;
    virtual int fd() = 0;
    virtual bool send_idle() = 0;
    virtual mpd_idle run_noidle() = 0;

    virtual QVector<const char *> search_db_tags(mpd_tag_type) = 0;
signals:
    void idle(mpd_idle);
};

#endif // ABSTRACTMPDCONNECTION_H

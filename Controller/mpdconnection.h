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
    mpd_connection *connection();

private:
    mpd_connection *m_mpd;
};

#endif // MPDCONNECTION_H

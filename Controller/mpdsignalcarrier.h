#ifndef MPDSIGNALCARRIER_H
#define MPDSIGNALCARRIER_H

#include <mpd/client.h>
#include <QObject>

class MPDSignalCarrier : public QObject
{
    Q_OBJECT
public:
    explicit MPDSignalCarrier(QString, unsigned, unsigned, QObject *parent = nullptr);
    mpd_connection *connection();

private:
    mpd_connection *m_mpd;
};

#endif // MPDSIGNALCARRIER_H

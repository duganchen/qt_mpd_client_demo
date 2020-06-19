#ifndef CONTROLLER_H
#define CONTROLLER_H

#if 0
#include "mpdsignalcarrier.h"
#endif
#include "Controller_global.h"
#include <mpd/client.h>
#include <QObject>
#include <QSocketNotifier>

class CONTROLLER_EXPORT Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QString, unsigned, unsigned, QObject *parent = nullptr);

    enum class ConnectionState { Disconnected, Connecting, Connected };
    Q_ENUM(ConnectionState)
public slots:
    void handleConnectClick();
    void handleListAlbumsClick();

public:
    QVector<QString> getAlbumList();

    QString host();
signals:
    void errorMessage(QString);
    void connectionState(Controller::ConnectionState connectionState);

    void requestConnection(QString, unsigned, unsigned);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

    void queueChanged();

private:
    void handleIdle(mpd_idle);
    void createMPD();

    QString m_host;
    unsigned m_port;
    unsigned m_timeout_ms;

    mpd_connection *m_connection;
    QSocketNotifier *m_notifier;
private slots:
#if 0
    void setMPD(MPDSignalCarrier *);
#endif
    void handleActivation();
};

Q_DECLARE_METATYPE(Controller::ConnectionState);

#endif // CONTROLLER_H

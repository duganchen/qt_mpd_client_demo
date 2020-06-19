#include "controller.h"
#include "mpdprocess.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtTest>

class TestConnection : public QObject
{
    Q_OBJECT

public:
    TestConnection();
    ~TestConnection();

private slots:
#if 0
    void test_spinUpMPD();
#endif
    void test_cannotConnect();
};

TestConnection::TestConnection() {}

TestConnection::~TestConnection() {}

void TestConnection::test_cannotConnect()
{
    auto controller = new Controller("locahost", 6600, 0);
    QSignalSpy spy(controller, &Controller::connectionState);
    controller->handleConnectClick();
    auto endState = spy.last()[0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Disconnected);
    delete controller;
}

#if 0
void TestConnection::test_spinUpMPD()
{
    auto proc = new MPDProcess();

    QCOMPARE(proc->mpdState(), QProcess::Running);
    if (proc->mpdState() != QProcess::Running) {
        return;
    }

    QCOMPARE(proc->mpdError(), MPD_ERROR_SUCCESS);
    if (proc->mpdError() != MPD_ERROR_SUCCESS) {
        return;
    }

    Controller controller(proc->socketPath().toUtf8().constData(), 0, 0);
    QSignalSpy spy(&controller, &Controller::connectionState);
    controller.handleConnectClick();
    spy.wait();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Connected);

    delete proc;

    spy.wait();
    endState = spy.takeLast()[0].value<Controller::ConnectionState>();
}
#endif

QTEST_MAIN(TestConnection)

#include "tst_testconnection.moc"

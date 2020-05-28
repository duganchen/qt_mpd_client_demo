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
    void test_spinUpMPD();
    void test_cannotConnect();
};

TestConnection::TestConnection() {}

TestConnection::~TestConnection() {}

void TestConnection::test_cannotConnect()
{
    auto controller = new Controller("localhost", 6600, 0);
    QSignalSpy spy(controller, &Controller::connectionState);
    controller->handleConnectClick();
    spy.wait();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Disconnected);
    delete controller;
}

void TestConnection::test_spinUpMPD()
{
    auto proc = new MPDProcess();
    QTest::qWait(MPD_START_MS);

    auto conn = mpd_connection_new(proc->socketPath().toUtf8().constData(), 0, 0);
    QVERIFY(conn);
    auto success = mpd_connection_get_error(conn);
    QVERIFY(MPD_ERROR_SUCCESS == success);
    QVERIFY(mpd_run_update(conn, nullptr));
    mpd_connection_free(conn);
    conn = nullptr;

    Controller controller(proc->socketPath().toUtf8().constData(), 0, 0);
    QSignalSpy spy(&controller, &Controller::connectionState);
    controller.handleConnectClick();
    spy.wait();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Connected);

    delete proc;

    spy.wait();
    endState = spy.takeLast()[0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Disconnected);
}

QTEST_MAIN(TestConnection)

#include "tst_testconnection.moc"

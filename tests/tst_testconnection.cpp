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
    // Note deliberate typo.
    auto controller = new Controller("locahost", 6600, 0);

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

QTEST_MAIN(TestConnection)

#include "tst_testconnection.moc"

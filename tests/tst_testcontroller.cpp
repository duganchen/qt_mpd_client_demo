#include "controller.h"
#include "mpdprocess.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtTest>

class TestController : public QObject
{
    Q_OBJECT

public:
    TestController();
    ~TestController();

private slots:
    void init();
    void cleanup();

    void test_theTest();

private:
    MPDProcess *m_mpdProcess;
};

TestController::TestController()
    : m_mpdProcess(nullptr)
{}

TestController::~TestController() {}

void TestController::init()
{
    qDebug() << "Init";
    m_mpdProcess = new MPDProcess(this);
}

void TestController::cleanup()
{
    qDebug() << "Cleanup";
    delete m_mpdProcess;
    m_mpdProcess = nullptr;
}

void TestController::test_theTest()
{
    QCOMPARE(m_mpdProcess->mpdState(), QProcess::Running);
    if (m_mpdProcess->mpdState() != QProcess::Running) {
        return;
    }

    QCOMPARE(m_mpdProcess->mpdError(), MPD_ERROR_SUCCESS);
    if (m_mpdProcess->mpdError() != MPD_ERROR_SUCCESS) {
        return;
    }

    Controller controller(m_mpdProcess->socketPath().toUtf8().constData(), 0, 1000);
    QSignalSpy spy(&controller, &Controller::connectionState);
    controller.handleConnectClick();
    spy.wait();
    auto albums = controller.getAlbumList();
    QCOMPARE(albums[0], "Touhou Luna Nights - Original Soundtrack");
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

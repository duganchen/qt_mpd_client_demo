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
    void test_theTest2();

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
    QVERIFY(true);
}

void TestController::test_theTest2()
{
    QVERIFY(true);
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

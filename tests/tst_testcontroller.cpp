#include "controller.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtTest>

// There's a delay between the time that you start MPD and when it
// starts accepting connections.This wait-time works reliably on my
// 2015 MacBook Pro.
constexpr int MPD_START_MS{500};

class TestController : public QObject
{
    Q_OBJECT

public:
    TestController();
    ~TestController();

private slots:
    void test_theTest();
};

TestController::TestController() {}

TestController::~TestController() {}

void TestController::test_theTest()
{
    QVERIFY(true);
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

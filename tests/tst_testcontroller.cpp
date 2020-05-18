#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class TestController : public QObject
{
	Q_OBJECT

public:
	TestController();
	~TestController();

private slots:
	void test_case1();

};

TestController::TestController()
{

}

TestController::~TestController()
{

}

void TestController::test_case1()
{

}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

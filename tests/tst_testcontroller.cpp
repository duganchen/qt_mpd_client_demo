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
    void test_spinUpMPD();
    void test_cannotConnect();
};

TestController::TestController() {}

TestController::~TestController() {}

void TestController::test_cannotConnect()
{
    auto controller = new Controller("localhost", 6600, 0);
    QSignalSpy spy(controller, &Controller::connectionState);
    controller->handleConnectClick();
    spy.wait();
    qDebug() << spy.count();
    qDebug() << spy[0][0].value<Controller::ConnectionState>();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Disconnected);
    delete controller;
}

void TestController::test_spinUpMPD()
{
    QFile templateFile{"test_resources/mpd.conf"};
    QVERIFY(templateFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in{&templateFile};
    QString tmplate{in.readAll()};
    tmplate = tmplate.arg(QCoreApplication::applicationDirPath() + "/test_resources/Music");
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    auto tempPath = dir.path();

    QVERIFY(QDir().mkdir(tempPath + "/playlists"));
    tmplate = tmplate.arg(tempPath);

    auto confPath = tempPath + "/mpd.conf";
    QFile confFile{confPath};
    QVERIFY(confFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream confStream(&confFile);
    confStream << tmplate;
    confFile.close();

    // For now, we don't need to copy the audio files to the temporary directory. The tests
    // don't modify them. If that changes, see:
    // https://forum.qt.io/topic/105993/copy-folder-qt-c

    QVERIFY(QFileInfo(tempPath).isDir());

    QProcess mpd;
    QStringList args;
    QVERIFY(QFileInfo(confPath).exists());
    args.append("-v");
    args.append("--no-daemon");
    args.append(confPath);

    mpd.setProcessChannelMode(QProcess::ForwardedChannels);

    // On OS X, you may need to go to Projects->Build and add
    // /usr/local/bin to the PATH in the build environment, if that's
    // where mpd is installed.
    mpd.start("mpd", args);
    QTest::qWait(MPD_START_MS);

    QCOMPARE(mpd.state(), QProcess::Running);

    auto socketPath = tempPath + "/socket";

    auto conn = mpd_connection_new(socketPath.toUtf8().constData(), 0, 0);

    QVERIFY(conn);
    auto success = mpd_connection_get_error(conn);
    if (success != MPD_ERROR_SUCCESS) {
        qDebug() << mpd_connection_get_error_message(conn);
    }
    QVERIFY(MPD_ERROR_SUCCESS == success);

    QVERIFY(mpd_run_update(conn, nullptr));

    if (!mpd_search_db_tags(conn, MPD_TAG_TITLE)) {
        qDebug() << mpd_connection_get_error_message(conn);
    }

    if (!mpd_search_commit(conn)) {
        qDebug() << mpd_connection_get_error_message(conn);
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(conn, MPD_TAG_TITLE)) != nullptr) {
        qDebug() << pair->value;
        mpd_return_pair(conn, pair);
    }

    QString a{"b"};

    qDebug() << "socketPath is " << socketPath.toUtf8().constData();
    Controller controller(socketPath.toUtf8().constData(), 0, 0);
    QSignalSpy spy(&controller, &Controller::connectionState);
    controller.handleConnectClick();
    spy.wait();
    qDebug() << spy.count();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Connected);

    mpd_connection_free(conn);

    args.clear();
    args.append("--kill");
    args.append(confPath);
    QProcess killer;
    killer.start("mpd", args);
    killer.waitForFinished();
    mpd.waitForFinished();
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

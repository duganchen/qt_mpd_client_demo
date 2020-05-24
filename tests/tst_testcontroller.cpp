#include "controller.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QDebug>
#include <QSignalSpy>
#include <QtTest>

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
    qRegisterMetaType<Controller::ConnectionState>();
    auto controller = new Controller("localhost", 6600, 0);
    QSignalSpy spy(controller, &Controller::connectionState);
    controller->handleConnectClick();
    spy.wait();
    qDebug() << spy.count();
    qDebug() << spy[0][0].value<Controller::ConnectionState>();
    auto endState = spy[0][0].value<Controller::ConnectionState>();
    QCOMPARE(endState, Controller::ConnectionState::Disconnected);
}

void TestController::test_spinUpMPD()
{
    qDebug() << "spinning up mpd";
    QFile templateFile{"test_resources/mpd.conf"};
    QVERIFY(templateFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in{&templateFile};
    QString tmplate{in.readAll()};
    tmplate = tmplate.arg(QCoreApplication::applicationDirPath() + "/test_resources/Music");
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    auto tempPath = dir.path();

    QDir().mkdir(tempPath + "/playlists");
    tmplate = tmplate.arg(tempPath);

    auto confPath = tempPath + "/mpd.conf";
    QFile confFile{confPath};
    QVERIFY(confFile.open(QIODevice::WriteOnly | QIODevice::Text));
    QTextStream confStream(&confFile);
    confStream << tmplate;

    confFile.close();

    qDebug() << tmplate.toUtf8().constData();

    /* A sample conf would be:
    music_directory		"/home/dugan/Documents/qt_mpd_client_demo/tests/build/test_resources/Music"
    playlist_directory		"/tmp/TestController-XFQcRT/playlists"
        db_file			"/tmp/TestController-XFQcRT/database"
        log_file			"/tmp/TestController-XFQcRT/log"
        pid_file			"/tmp/TestController-XFQcRT/pid"
        bind_to_address				"/tmp/TestController-XFQcRT/socket"
        */

    // For now, we don't need to copy the audio files to the temporary directory. The tests
    // don't modify them. If that changes, see:
    // https://forum.qt.io/topic/105993/copy-folder-qt-c

    QProcess mpd;
    QStringList args;
    args.append(confPath);
    mpd.start("mpd", args);
    mpd.waitForFinished();

    auto socketPath = tempPath + "/socket";

    qDebug() << tempPath.toUtf8().constData();

    auto conn = mpd_connection_new(socketPath.toUtf8().constData(), 0, 0);

    QVERIFY(conn);

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

    mpd_connection_free(conn);

    args.clear();
    args.append("--kill");
    args.append(confPath);
    mpd.start("mpd", args);
    mpd.waitForFinished();
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

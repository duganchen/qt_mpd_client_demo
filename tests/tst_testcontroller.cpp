#include <QCoreApplication>
#include <QDebug>
#include <QtTest>
#include <mpd/client.h>

class TestController : public QObject
{
    Q_OBJECT

public:
    TestController();
    ~TestController();

private slots:
    void test_spinUpMPD();
};

TestController::TestController()
{
}

TestController::~TestController()
{
}

void TestController::test_spinUpMPD()
{
    QFile templateFile{"resources/mpd.conf"};
    QVERIFY(templateFile.open(QIODevice::ReadOnly | QIODevice::Text));
    QTextStream in{&templateFile};
    QString tmplate{in.readAll()};
    tmplate = tmplate.arg(QCoreApplication::applicationDirPath() + "/resources/Music");
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

    /* A sample conf would be:
     music_directory		"/home/dugan/Documents/qt_mpd_client_demo/tests/build/resources/Music"
     playlist_directory		"/tmp/TestController-XFQcRT/playlists"
    db_file			"/tmp/TestController-XFQcRT/database"
    log_file			"/tmp/TestController-XFQcRT/log"
    pid_file			"/tmp/TestController-XFQcRT/pid"
	bind_to_address				"/tmp/TestController-XFQcRT/socket"
    */

    QProcess mpd;
    QStringList args;
    args.append(confPath);
    mpd.start("mpd", args);
    mpd.waitForFinished();

	auto socketPath = tempPath + "/socket";

	auto conn = mpd_connection_new(tempPath.toUtf8().constData(), 0, 0);

    if (!mpd_search_db_tags(conn, MPD_TAG_TITLE))
    {
        qDebug() << mpd_connection_get_error_message(conn);
    }

    if (!mpd_search_commit(conn))
    {
        qDebug() << mpd_connection_get_error_message(conn);
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(conn, MPD_TAG_TITLE)) != nullptr)
    {
        qDebug() << pair->value;
        mpd_return_pair(conn, pair);
    }

    mpd_connection_free(conn);

    args.clear();
    args.append("--kill");
    args.append(confPath);
    mpd.start("mpd", args);
    mpd.waitForFinished();
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"

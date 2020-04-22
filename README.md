# Demo: Qt MPD Client Architecture

This is a demo of how I would approach writing [Qt](https://www.qt.io/) code that communicates with an [MPD](https://www.musicpd.org/) server.

I use [libmpdclient](https://www.musicpd.org/libs/libmpdclient/) instead of trying to implement my own data access layer.

In CMake, I specify the languages as both C (libmpdclient) and C++ (Qt), and I use pkgconfig to set up the libmpdclient dependencies.

Using libmpdclient, I create two connection instances: one to synchronously send commands and immediately receive their output, and the other to asynchronously handle MPD "idle" command, which subscribes to push notifications. These are pointers to malloc'd C structs, so I allow a QObject to control their lifecycle.

To know when idle notifications have arrived, I hook the asynchronous connection's file descriptor (which libmpdclient exposes) up to a [QSocketNotifier](https://doc.qt.io/qt-5/qsocketnotifier.html).

The QObject subclass is therefore a Facade that coordinates two mpd_connections and a QSocketNotifier. Need a new MPD connection? Delete it and instantiate a new one.

If you try to create an mpd_connection instance with a host that can't be resolved (say, if you typo localhost as locahost), then the mpd_connection_new call can block for a long time. How to handle that? Honestly, I just use [QHostInfo](https://doc.qt.io/qt-5/qhostinfo.html) to asynchronously check that the host is resolvable first.

Note that with this approach, I do not need to worry about parsing data sent from MPD (which, incidentally, is in a format that would parse extremely well with awk), escaping data sent to MPD (which MPD's documentation specifically advises against reimplementing), or multithreading. It also takes care of, as far as I can tell, every case where blocking would actually be a problem.

As for the Qt side? I start with the standard CMake QWidgets boilerplate created by Qt Creator, and I add a button to "list albums" All output from MPD goes into QDebug. The presentation layer is not what I'm demonstrating.
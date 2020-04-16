# Demo: Qt MPD Client Architecture

This is a demo of how I would approach writing [Qt](https://www.qt.io/) code that communicates with an [MPD](https://www.musicpd.org/) server.

I start with the standard CMake QWidgets boilerplate created by Qt Creator, and I add a button to "list albums" All output from MPD goes into QDebug. The presentation layer is not what I'm demonstrating.

I use [libmpdclient](https://www.musicpd.org/libs/libmpdclient/) instead of trying to implement my own data access layer.

In CMake, I specify the languages as both C (libmpdclient) and C++ (Qt), and I use pkgconfig to set up the libmpdclient dependencies.

Using libmpdclient, I create two connection instances: one to synchronously send commands and immediately receive their output, and the other to asynchronously handle MPD "idle" command, which subscribes to push notifications. These are pointers to malloc'd C structs, so I wrap them in Qt's smart pointers.

To know when idle notifications have arrived, I hook the asynchronous connection's file descriptor (which libmpdclient exposes) up to a [QSocketNotifier](https://doc.qt.io/qt-5/qsocketnotifier.html).


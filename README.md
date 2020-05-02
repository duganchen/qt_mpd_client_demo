# Demo: Qt MPD Client Architecture

This is a demo of how I would approach writing [Qt](https://www.qt.io/) code that communicates with an [MPD](https://www.musicpd.org/) server.

I use [libmpdclient](https://www.musicpd.org/libs/libmpdclient/) instead of trying to implement my own data access layer, whose scope would typically include connecting to MPD via a socket, [escaping commands sent to MPD](https://www.musicpd.org/doc/html/protocol.html#escaping-string-values), and parsing MPD's output .

In CMake, I specify the languages as both C (libmpdclient) and C++ (Qt). I tell CMake to use pkgconfig to locate libmpdclient.

The proper way to integrate MPD into a program's architecture is described as follows:

> mpd_send_idle(), then register libmpdclient's socket (see
> mpd_connection_get_fd()) in your I/O event loop.
>
> As soon as MPD sends the result, your event loop triggers your
> callback, and you can use mpd_recv_idle() to read the result.  After
> you're done handling those idle events, re-enter idle.
>
> If you want to interact with MPD in the meantime, call
> mpd_run_noidle() and handle idle events which may have occurred; then
> send the desired commands to MPD.  Finally re-enter idle.
>
> This is not magic.  And it's not specific to MPD.  This is basic
> event-based programming.  Just one connection and one thread, no
> latencies, no timeouts, no waiting.

[Re: \[mpd-devel] Proper Use of IDLE](https://www.mail-archive.com/mpd-devel@musicpd.org/msg00641.html)

Using libmpdclient, I create a single connection.

To know when idle notifications have arrived, I hook the asynchronous connection's file descriptor (which libmpdclient exposes) up to a [QSocketNotifier](https://doc.qt.io/qt-5/qsocketnotifier.html).

Idle notifications are received asynchronously, and commands are sent and received synchronously. When I need to send a command, the process is:

1. disable the socket notifier
2. send noidle, receive and process the results
3. send the command and receive the results
4. turn the socket notifier back on
5. send idle

Note that the idle notifications tell you when you've lost the connection. When that happens, you receive a notification with the value of 0, and you get "Connection closed by the server" as an error. The error code constant is MPD_ERROR_CLOSED.

For reconnections? One option is to make both the MPD connection instance and the socket notifier and instance of a Facade class. When you need a new connection, free it and instantiate a new one.

With this setup, the only place where I've seen blocking being an issue is when libmpdclient connects. And the part of the connection process where it's an issue is the host address resolution (I checked). I dealt with it by using Qt's [QHostInfo::lookupHost](https://doc.qt.io/qt-5/qhostinfo.html#lookupHost-1) to validate the hostname before connecting MPD.

As for the Qt side? I start with the standard CMake QWidgets boilerplate created by Qt Creator, and I add a button to "list albums" All output from MPD goes into QDebug. The presentation layer is not what I'm demonstrating.
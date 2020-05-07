# Demo: Qt MPD Client Architecture

This is a demo of how I would approach writing [Qt](https://www.qt.io/) code that communicates with an [MPD](https://www.musicpd.org/) server.

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

I use [libmpdclient](https://www.musicpd.org/libs/libmpdclient/) instead of trying to implement my own data access layer, whose scope would typically include connecting to MPD via a socket, [escaping commands sent to MPD](https://www.musicpd.org/doc/html/protocol.html#escaping-string-values), and parsing MPD's output .

In CMake, I specify the languages as both C (libmpdclient) and C++ (Qt). I tell CMake to use pkgconfig to locate libmpdclient.

For the code architecture, here's the general approach.

To know when idle notifications have arrived, I hook the  connection's file descriptor (which libmpdclient exposes) up to a [QSocketNotifier](https://doc.qt.io/qt-5/qsocketnotifier.html).

Idle notifications are received asynchronously, and commands are sent and received synchronously. When I need to send a command, the process is:

1. disable the socket notifier
2. send noidle, receive and process the results
3. send the command and receive the results
4. turn the socket notifier back on
5. send idle

Note that the idle notifications tell you when you've lost the connection. When that happens, you receive a notification with the value of 0, and you get "Connection closed by the server" as an error. The error code constant is MPD_ERROR_CLOSED.

Now, Creating the connection is  is done using **mpd_connection_new**, which can block until a programmer-specified timeout is reached, and which returns a pointer to an opaque object. This is actually one of the trickier parts to integrate, as:

* it can block for long enough for it to matter
* the object, being a "pointer to an incomplete type", cannot be passed via Qt's signals and slots

So here's the general architecture.

I have a QObject subclass that encapsulates both an ***mpd_connection*** object and its socket notifier. In another thread, I have a [Factory](https://en.wikipedia.org/wiki/Factory_method_pattern) that creates these MPD connection instances and moves them back to the main thread. Data communications between the main thread and the factory is done with signals and slots. Connecting to MPD is the only part that's multithreaded. The rest is event-based.

When an idle notification comes back, you can tell if you've lost the connection. And then you can request another connection from the factory.
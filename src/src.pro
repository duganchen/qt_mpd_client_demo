TARGET = qt_mpd_client_demo

QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        connectionmanager.cpp \
        controller.cpp \
        main.cpp \
        mainwindow.cpp \
        mpdconnection.cpp \
        mpdconnectionfactory.cpp

HEADERS += \
    connectionmanager.h \
    connectionstate.h \
    controller.h \
    mainwindow.h \
    mpdconnection.h \
    mpdconnectionfactory.h


unix:!macx: LIBS += -lmpdclient

macx: INCLUDEPATH += /usr/local/Cellar/libmpdclient/2.18/include
macx: LIBS += -lmpdclient.2 -L/usr/local/Cellar/libmpdclient/2.18/lib

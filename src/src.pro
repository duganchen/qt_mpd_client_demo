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
        main.cpp \
        mainwindow.cpp

HEADERS += \
    mainwindow.h

INCLUDEPATH += ../Controller

# https://stackoverflow.com/a/20805469/240515
mac {
  PKG_CONFIG = /usr/local/bin/pkg-config
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libmpdclient

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Controller/release/ -lController
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Controller/debug/ -lController
else:unix: LIBS += -L$$OUT_PWD/../Controller/ -lController

INCLUDEPATH += $$PWD/../Controller
DEPENDPATH += $$PWD/../Controller

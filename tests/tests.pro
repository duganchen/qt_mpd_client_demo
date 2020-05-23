QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES += \
	tst_testcontroller.cpp

INCLUDEPATH += ../Controller

# https://stackoverflow.com/a/20805469/240515
mac {
  PKG_CONFIG = /usr/local/bin/pkg-config
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libmpdclient

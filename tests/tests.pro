QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES += \
	tst_testcontroller.cpp

INCLUDEPATH += ../Controller

unix:!macx: LIBS += -lmpdclient

macx: INCLUDEPATH += /usr/local/Cellar/libmpdclient/2.18/include
macx: LIBS += -lmpdclient.2 -L/usr/local/Cellar/libmpdclient/2.18/lib

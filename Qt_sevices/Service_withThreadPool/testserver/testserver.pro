#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T14:57:40
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = testserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    dataservice.cpp \
    datatcpserver.cpp \
    taskfortcpserver.cpp
include(qtservice/src/qtservice.pri)
#include(log4qt/log4qt.pri)

HEADERS += \
    dataservice.h \
    datatcpserver.h \
    taskfortcpserver.h \
    DataLogger.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/release/ -llog4cxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/local/lib/debug/ -llog4cxx
else:unix: LIBS += -L$$PWD/../../../usr/local/lib/ -llog4cxx

INCLUDEPATH += $$PWD/../../../usr/local/include
DEPENDPATH += $$PWD/../../../usr/local/include

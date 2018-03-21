#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T19:42:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grpcClient
TEMPLATE = app

DEFINES += GRPC_USE_DLLS
DEFINES += PROTOBUF_USE_DLLS

LIBS +=  $$PWD/../../lib/grpc++.lib
LIBS +=   $$PWD/../../lib/libprotobufd.lib

SOURCES += main.cpp\
        grpctest.cpp \
     ../../proto/grpctest.grpc.pb.cc \
    ../../proto/grpctest.pb.cc

INCLUDEPATH +=   ../../grpc/include
INCLUDEPATH +=  ../../grpc/.build/third_party/protobuf/include
INCLUDEPATH +=  $$PWD/../../proto



FORMS    += grpctest.ui



HEADERS += ../../proto/grpctest.grpc.pb.h \
    ../../proto/grpctest.pb.h \
    grpctest.h


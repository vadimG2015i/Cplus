QT += core
QT -= gui

CONFIG += c++11

TARGET = grpcSrv
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


LIBS +=  $$PWD/../lib/grpc++.lib
LIBS +=   $$PWD/../lib/libprotobufd.lib


INCLUDEPATH +=  D:/pro/grpc/include
INCLUDEPATH +=  $$PWD/../../grpc/include
INCLUDEPATH +=  ../proto
INCLUDEPATH +=  D:/pro/grpc/.build/third_party/protobuf/include

INCLUDEPATH += $$PWD/../../../grpc/cmake/third_party/protobuf/include

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += GRPC_USE_DLLS
DEFINES += PROTOBUF_USE_DLLS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ../proto/grpctest.grpc.pb.cc \
    ../proto/grpctest.pb.cc

HEADERS += \
    ../proto/grpctest.grpc.pb.h \
    ../proto/grpctest.pb.h




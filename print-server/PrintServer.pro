QT += core gui printsupport

CONFIG += c++11

TARGET = PrintServer
CONFIG += console
CONFIG -= app_bundle

LIBS +=  -lboost_system-mgw49-mt-1_61 -lws2_32

TEMPLATE = app

SOURCES += main.cpp \
        tcpclient.cpp \
        tcpserver.cpp \
        printserver.cpp \
    app.cpp \
    configs.cpp \
    log.cpp

HEADERS += tcpclient.h \
        tcpserver.h \
        printserver.h \
    app.h \
    configs.h \
    log.h

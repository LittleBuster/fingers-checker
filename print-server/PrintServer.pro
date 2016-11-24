QT += core gui printsupport

CONFIG += c++11

TARGET = PrintServer
CONFIG += console
CONFIG -= app_bundle

LIBS += -lws2_32

TEMPLATE = app

SOURCES += main.cpp \
        tcpclient.cpp \
        tcpserver.cpp \
        printserver.cpp \
        app.cpp \
        configs.cpp \
        log.cpp \
        ext.cpp

HEADERS += tcpclient.h \
        tcpserver.h \
        printserver.h \
        app.h \
        configs.h \
        log.h \
        ext.h

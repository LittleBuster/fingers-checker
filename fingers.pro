QT += core gui printsupport sql

CONFIG += c++11

TARGET = fingers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lcurl -lboost_system -lws2_32

SOURCES += main.cpp \
            app.cpp \
            log.cpp \
    checker.cpp \
    database.cpp \
    printer.cpp

HEADERS += \
    app.h \
    log.h \
    checker.h \
    database.h \
    printer.h

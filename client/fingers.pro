QT += core gui printsupport sql

CONFIG += c++11

TARGET = fingers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lcurl -lboost_system -lpthread

SOURCES += main.cpp \
            app.cpp \
            log.cpp \
            checker.cpp \
    configs.cpp

HEADERS += \
    app.h \
    log.h \
    checker.h \
    configs.h

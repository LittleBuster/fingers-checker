QT += core gui printsupport sql

CONFIG += c++11

TARGET = fingers
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lcurl -lboost_system -lpthread -lmysqlclient -lboost_thread

SOURCES += main.cpp \
            app.cpp \
            log.cpp \
            checker.cpp \
            configs.cpp \
            database.cpp \
            notify.cpp \
            tcpclient.cpp \
    threadmanager.cpp \
    devicechecker.cpp

HEADERS += \
    app.h \
    log.h \
    checker.h \
    configs.h \
    database.h \
    notify.h \
    tcpclient.h \
    threadmanager.h \
    devicechecker.h \
    ichecker.h

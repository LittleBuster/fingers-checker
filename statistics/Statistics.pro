#-------------------------------------------------
#
# Project created by QtCreator 2016-07-04T08:44:21
#
#-------------------------------------------------

QT += core gui widgets

TARGET = Statisctics
TEMPLATE = app

LIBS += -lboost_system-mgw49-1_61 -lws2_32 -lssl -lcrypto "C:\\Qt\\Tools\\mingw530_32\\i686-w64-mingw32\\bin\\libmysql.dll"

SOURCES += main.cpp\
        mainwindow.cpp \
    app.cpp \
    qapp.cpp \
    tcpclient.cpp \
    database.cpp \
    configs.cpp \
    log.cpp \
    helper.cpp

HEADERS  += mainwindow.h \
    app.h \
    qapp.h \
    tcpclient.h \
    database.h \
    configs.h \
    log.h \
    helper.h

FORMS    += mainwindow.ui

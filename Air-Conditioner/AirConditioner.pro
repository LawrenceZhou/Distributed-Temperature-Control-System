#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T20:26:35
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AirConditioner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    room.cpp

HEADERS  += mainwindow.h \
    login.h \
    room.h

FORMS    += mainwindow.ui \
    login.ui

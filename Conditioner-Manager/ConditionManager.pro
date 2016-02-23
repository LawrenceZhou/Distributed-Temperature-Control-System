#-------------------------------------------------
#
# Project created by QtCreator 2015-05-22T10:10:57
#
#-------------------------------------------------

QT       += core gui\
          network\
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConditionManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    DataPact.cpp \
    database.cpp \
    recordview.cpp

HEADERS  += mainwindow.h \
    login.h \
    DataPact.h \
    database.h \
    recordview.h

FORMS    += mainwindow.ui \
    login.ui \
    recordview.ui




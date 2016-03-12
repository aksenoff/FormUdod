#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T09:53:38
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KcttTempDB
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectiondialog.cpp

HEADERS  += mainwindow.h \
    connectiondialog.h

FORMS    += mainwindow.ui \
    connectiondialog.ui

RESOURCES += \
    sourses.qrc

CONFIG += c++11

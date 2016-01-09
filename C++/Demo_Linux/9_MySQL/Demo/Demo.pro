#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T09:05:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -LD:\MySQL\lib -llibmysql

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindlg.cpp \
    scriptdlg.cpp \
    mymysql.cpp \
    insertdlg.cpp

HEADERS  += mainwindow.h \
    logindlg.h \
    scriptdlg.h \
    mymysql.h \
    insertdlg.h

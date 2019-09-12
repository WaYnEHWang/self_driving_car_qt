#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T13:06:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atuoset
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widget.cpp

HEADERS  += mainwindow.h \
    widget.h \
    workthread.h \
    checkdockerthread.h

FORMS    += mainwindow.ui

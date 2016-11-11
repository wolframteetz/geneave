#-------------------------------------------------
#
# Project created by QtCreator 2016-11-11T19:10:40
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = geneave
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    serial.h

FORMS    += mainwindow.ui \
    serial.ui

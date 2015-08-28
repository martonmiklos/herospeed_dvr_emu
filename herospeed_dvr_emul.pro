#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T20:03:35
#
#-------------------------------------------------

QT       += core gui network

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

DESTDIR = bin


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = herospeed_dvr_emul
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

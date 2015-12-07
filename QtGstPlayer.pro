#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T12:09:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGstPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    playergui.cpp

HEADERS  += mainwindow.h \
    player.h \
    playergui.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 Qt5GStreamerUi-1.0

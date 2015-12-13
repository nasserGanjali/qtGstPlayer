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

LIBS += -L/home/nasser/project/buildroot/buildroot-2015.05/output/rootfs_nfs/usr/lib
INCLUDEPATH+=/home/nasser/project/buildroot/buildroot-2015.05/output/rootfs_nfs/usr/include/Qt5GStreamer
CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 Qt5GStreamerUi-1.0

FORMS    += mainwindow.ui


#QMAKE_CXXFLAGS += -I=/usr/include/Qt5GStreamer

#LIBS += -lQt5GLib-2.0 -lQt5GStreamer-1.0 -lQt5GStreamerUi-1.0
#INCLUDEPATH += =/usr/include/Qt5GStreamer

target.path+=/usr/bin
INSTALLS+=target

#QMAKE_CXXFLAGS += -O0

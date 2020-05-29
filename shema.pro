#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:22:53
#
#-------------------------------------------------

QT       += core gui network widgets

TARGET = shema
TEMPLATE = app

MVP_ROOT=$$PWD/..

DEFINES +=NO_LIBRARY

MVP_WORK_ROOT=$$MVP_ROOT/mvp_work
#https://github.com/yx500/mvp_classes.git
include($$MVP_ROOT/mvp_classes/mvp_all.pri)
#https://github.com/yx500/common_src.git
include($$MVP_ROOT/common_src/signalmanager/signalmanager.pri)
include($$MVP_ROOT/common_src/archivemanager/archivemanager.pri)




SOURCES += main.cpp\
        mainwindow.cpp \
    activegraphicsview.cpp

HEADERS  += mainwindow.h \
    activegraphicsview.h

FORMS    += mainwindow.ui

win32 {
        QMAKE_LFLAGS += -static-libgcc
        QMAKE_LFLAGS += -static
}

DISTFILES +=

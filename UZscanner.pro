#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T16:04:19
#
#-------------------------------------------------

QT    += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UZscanner
TEMPLATE = app


SOURCES += main.cpp\
        uzmainwindow.cpp \
    uzapplication.cpp \
    networkmanager.cpp \
    lineedit.cpp \
    windowpages.cpp \
    searchparameters.cpp

HEADERS  += uzmainwindow.h \
    uzapplication.h \
    networkmanager.h \
    lineedit.h \
    requestdata.h \
    train.h \
    windowpages.h \
    searchparameters.h

FORMS    += uzmainwindow.ui

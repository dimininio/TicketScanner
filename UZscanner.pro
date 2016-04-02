#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T16:04:19
#
#-------------------------------------------------

QT    += core gui network webkitwidgets webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UZscanner
TEMPLATE = app


SOURCES += main.cpp\
        uzmainwindow.cpp \
    uzapplication.cpp \
    networkmanager.cpp \
    lineedit.cpp \
    windowpages.cpp \
    searchparameters.cpp \
    widgetsmediator.cpp \
    animatedsearchwidget.cpp

HEADERS  += uzmainwindow.h \
    uzapplication.h \
    networkmanager.h \
    lineedit.h \
    requestdata.h \
    train.h \
    windowpages.h \
    searchparameters.h \
    widgetsmediator.h \
    animatedsearchwidget.h

FORMS    += uzmainwindow.ui

RESOURCES += \
    res.qrc

CONFIG+=c++11

DISTFILES +=

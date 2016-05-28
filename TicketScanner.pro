#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T16:04:19
#
#-------------------------------------------------

QT    += core gui network webkitwidgets webkit multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TicketScanner
TEMPLATE = app


SOURCES += main.cpp\
    uzapplication.cpp \
    networkmanager.cpp \
    lineedit.cpp \
    windowpages.cpp \
    searchparameters.cpp \
    widgetsmediator.cpp \
    animatedsearchwidget.cpp \
    requestdata.cpp \
    config.cpp \
    windowtitlebar.cpp \
    uzmainwindow.cpp \
    windowwrapper.cpp


HEADERS  += \
    uzapplication.h \
    networkmanager.h \
    lineedit.h \
    requestdata.h \
    train.h \
    windowpages.h \
    searchparameters.h \
    widgetsmediator.h \
    animatedsearchwidget.h \
    config.h \
    windowtitlebar.h \
    uzmainwindow.h \
    windowwrapper.h

FORMS    +=

RESOURCES += \
    res.qrc

CONFIG+=c++11

DISTFILES +=

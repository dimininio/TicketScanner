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
    networkmanager.cpp \
    lineedit.cpp \
    windowpages.cpp \
    searchparameters.cpp \
    widgetsmediator.cpp \    
    requestdata.cpp \
    windowtitlebar.cpp \
    uzmainwindow.cpp \
    windowwrapper.cpp \
    parser.cpp \
    message.cpp \
    application.cpp \
    ui/mainwindow.cpp \
    ui/browserpage.cpp \
    ui/settingspage.cpp \
    ui/scanningpage.cpp \
    ui/animatedsearchwidget.cpp


HEADERS  += \
    networkmanager.h \
    lineedit.h \
    requestdata.h \
    train.h \
    windowpages.h \
    searchparameters.h \
    widgetsmediator.h \
    config.h \
    windowtitlebar.h \
    uzmainwindow.h \
    windowwrapper.h \
    parser.h \
    tooltips.h \
    message.h \
    application.h \
    ui/mainwindow.h \
    ui/browserpage.h \
    ui/settingspage.h \
    ui/scanningpage.h \
    ui/animatedsearchwidget.h

FORMS    += \
    ui/mainwindow.ui \
    ui/browserpage.ui \
    ui/settingspage.ui \
    ui/scanningpage.ui

RESOURCES += \
    res.qrc

CONFIG+=c++11

win32: RC_ICONS = resources\TicketScanner.ico
macx: ICON = resources\TicketScanner.icns


VERSION = 1.0
QMAKE_TARGET_COPYRIGHT = dimininio


DISTFILES +=

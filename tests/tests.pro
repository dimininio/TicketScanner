#-------------------------------------------------
#
# Project created by QtCreator 2016-07-10T23:22:07
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_teststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    test_main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += $$PWD/../TicketScanner


HEADERS += $$PWD/../TicketScanner/parser.h
SOURCES += $$PWD/../TicketScanner/parser.cpp

CONFIG+=c++11

DISTFILES +=

RESOURCES += \
    resources.qrc

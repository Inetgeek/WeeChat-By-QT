#-------------------------------------------------
#
# Project created by QtCreator 2022-10-28T21:39:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET = WeeChat
TEMPLATE = app
DESTDIR = $$PWD/../bin
CONFIG += warn_off

SOURCES += main.cpp\
    menubutton.cpp \
    messages.cpp \
    search.cpp \
    tcpclient.cpp \
    tcpserver.cpp \
    widget.cpp \
    frmemailtool.cpp \
    sendemailthread.cpp


HEADERS  += widget.h \
    config.h \
    messages.h \
    search.h \
    menubutton.h \
    tcpclient.h \
    tcpserver.h \
    frmemailtool.h \
    sendemailthread.h

FORMS    += widget.ui \
    messages.ui \
    tcpclient.ui \
    tcpserver.ui \
    frmemailtool.ui

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../3rd_smtpclient
include ($$PWD/../3rd_smtpclient/3rd_smtpclient.pri)

RESOURCES += \
    res.qrc

RC_FILE += favicon.rc

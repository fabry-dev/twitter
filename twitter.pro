#-------------------------------------------------
#
# Project created by QtCreator 2018-09-28T08:37:50
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets serialport
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = twitter
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += main.cpp \
    webwindow.cpp \
    serialwatcher.cpp



HEADERS  += \
    webwindow.h \
    serialwatcher.h \
    slabel.h



FORMS    +=


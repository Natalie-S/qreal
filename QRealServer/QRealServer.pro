#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T21:33:30
#
#-------------------------------------------------

QT       += core \
    network
QT       -= gui

TARGET = QRealServer

QMAKE_CXXFLAGS += -std=c++11

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    socketwrapper.cpp

HEADERS += \
    server.h \
    socketwrapper.h

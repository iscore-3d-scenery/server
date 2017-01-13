QT += core
QT += network
QT += widgets
QT -= gui
CONFIG += c++11

TARGET = Project_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp

HEADERS += \
    server.h


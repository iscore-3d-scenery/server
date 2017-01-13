QT += core
QT += network
QT += widgets
CONFIG += c++14
QMAKE_CXXFLAGS+=-std=c++14
TARGET = Project_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h


# Qt QMake project file for OpenCAN LinCAN device plugin

include(../../opencan.pri)

QT += core

TARGET = LinCAN
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../

# OpenCAN include and library paths
INCLUDEPATH = ../../include
LIBS = -lOpenCAN -L../../

SOURCES += oclincanplugin.cpp \
    oclincandevice.cpp
HEADERS += oclincanplugin.h \
    oclincandevice.h \
    lincan.h

OTHER_FILES += \
    LinCAN.json

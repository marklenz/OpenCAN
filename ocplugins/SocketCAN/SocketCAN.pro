# Qt QMake project file for OpenCAN SocketCAN device plugin

include(../../opencan.pri)

QT += core

TARGET = SocketCAN
TEMPLATE = lib
CONFIG += plugin
win32:CONFIG -= embed_manifest_dll

# OpenCAN include and library paths
INCLUDEPATH = ../../include
LIBS = -lOpenCAN -L../../
win32:LIBS += ../../OpenCAN.lib

DESTDIR = ../

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

SOURCES +=\
    ocsocketcanplugin.cpp \
    ocsocketcandevice.cpp

HEADERS  += \
    ocsocketcanplugin.h \
    ocsocketcandevice.h

DISTFILES += \
    SocketCAN.json



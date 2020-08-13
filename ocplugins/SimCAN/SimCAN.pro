# Qt QMake project file for OpenCAN SimCAN IPC device plugin

include(../../opencan.pri)

QT += core

TARGET = SimCAN
TEMPLATE = lib
CONFIG += plugin
win32:CONFIG -= embed_manifest_dll

# OpenCAN include and library paths
INCLUDEPATH = ../../include
LIBS = -lOpenCAN -L../../
win32:LIBS += ../../OpenCAN.lib

DESTDIR = ../

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

SOURCES += \
    ocsimcandevice.cpp \
    ocsimcanmessagebuffer.cpp \
    ocsimcanplugin.cpp

HEADERS += \
    ocsimcandevice.h \
    ocsimcanmessagebuffer.h \
    ocsimcanplugin.h

OTHER_FILES += \
    SimCAN.json



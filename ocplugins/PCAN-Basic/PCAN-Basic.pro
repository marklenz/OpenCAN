# Qt QMake project file for OpenCAN PCAN-Basic device plugin
#
# NOTE: You need to set the include and library paths below
#       to the paths where you have the PCAN-Basic API
#       installed to compile on Windows, i.e., the 2 lines
#       starting with win32:INCLUDEPATH and win32:LIBS in
#       the PCAN-Basic includes and library section.

include(../../opencan.pri)

QT += core

TARGET = PCAN-Basic
TEMPLATE = lib
CONFIG += plugin
win32:CONFIG -= embed_manifest_dll

# PCAN-Basic API include and library paths
win32:INCLUDEPATH += "C:/PEAK/PCAN-Basic API/Include"
win32:LIBS += "C:/PEAK/PCAN-Basic API/Win32/VC_LIB/PCANBasic.lib"

# OpenCAN include and library paths
INCLUDEPATH += ../../include
LIBS += -lOpenCAN -L../../
win32:LIBS += ../../OpenCAN.lib

DESTDIR = ../

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

SOURCES += \
    ocpcanbasicplugin.cpp \
    ocpcanbasicdevice.cpp
HEADERS += \
    ocpcanbasicplugin.h \
    ocpcanbasicdevice.h

OTHER_FILES += \
    PCAN-Basic.json


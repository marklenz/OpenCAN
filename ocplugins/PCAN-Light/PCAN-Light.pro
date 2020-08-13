# Qt QMake project file for OpenCAN PCAN-Light device plugin
#
# NOTE: You need to set the include and library paths below
#       to the paths where you have the PCAN-Light API
#       installed to compile on Windows, i.e., the 2 lines
#       starting with win32:INCLUDEPATH and win32:LIBS in
#       the PCAN-Light includes and library section.

include(../../opencan.pri)

QT += core

TARGET = PCAN-Light
TEMPLATE = lib
CONFIG += plugin
win32:CONFIG -= embed_manifest_dll

# PCAN-Light include and library paths
win32:INCLUDEPATH += "C:/PEAK/PCAN-Light API/Include/C++"
win32:LIBS += "C:/PEAK/PCAN-Light API/Win32/VC_LIB/Pcan_usb.lib"
unix:LIBS += -lpcan -L/usr/lib

# OpenCAN include and library paths
INCLUDEPATH += ../../include
LIBS += -lOpenCAN -L../../
win32:LIBS += ../../OpenCAN.lib

DESTDIR = ../

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

SOURCES += \
    ocpcanlightdevice.cpp \
    ocpcanlightplugin.cpp
HEADERS += \
    ocpcanlightdevice.h \
    ocpcanlightplugin.h

OTHER_FILES += \
    PCAN-Light.json

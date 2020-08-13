# Qt QMake project file for OpenCAN library

include(opencan.pri)

QT += core

TARGET = OpenCAN
TEMPLATE = lib
CONFIG += shared

DESTDIR = ../OpenCAN

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

# Set the OPENCAN_LIBRARY #define to export functions in the shared library
DEFINES += OPENCAN_LIBRARY

# Set the global OpenCAN directory
win32:DEFINES += OPENCAN_DIR=\\\"C:\\\OpenCAN\\\plugins\\\"
unix:DEFINES += OPENCAN_DIR=\"\\\"/usr/lib64/OpenCAN/plugins\\\"\"

INCLUDEPATH += include

HEADERS += include/ocdevicemanager.h \
    include/ocdevice.h \
    include/ocmessage.h \
    include/ocdeviceinterface.h \
    include/occore.h \
    include/ocplugininfo.h \
    include/ocdoc.h

SOURCES += src/ocdevicemanager.cpp \
    src/ocdevice.cpp \
    src/ocmessage.cpp

OTHER_FILES += \
    opencan.pri \
    qtversion.pri \
    qtperf.pri \
    README.txt \
    CHANGELOG.txt


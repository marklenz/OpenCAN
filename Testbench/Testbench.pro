# Qt QMake project file for OpenCAN application Testbench

include(../opencan.pri)

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = testbench
macx: TARGET = Testbench    # Qt uses TARGET to set the application name on Mac OS X

# OpenCAN include and library paths
INCLUDEPATH = ../include
LIBS = -lOpenCAN -L../
win32:LIBS += ../OpenCAN.lib

DESTDIR = ../

win32:DEFINES += _BIND_TO_CURRENT_CRT_VERSION

SOURCES += basespinbox.cpp \
    main.cpp \
    multisendwindow.cpp \
    receivewindow.cpp \
    sendmessagewidget.cpp \
    testbench.cpp \
    sendwindow.cpp \
    managementwindow.cpp \
    plugindialog.cpp \
    deviceselector.cpp

HEADERS += basespinbox.h \
    multisendwindow.h \
    receivewindow.h \
    sendmessagewidget.h \
    testbench.h \
    sendwindow.h \
    managementwindow.h \
    plugindialog.h \
    deviceselector.h


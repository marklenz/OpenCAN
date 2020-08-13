# OpenCAN QMake project include - macros and build settings shared across all OpenCAN components

!isEmpty(OPENCAN_PRI_INCLUDED): error("opencan.pri already included")
OPENCAN_PRI_INCLUDED = 1

!include(qtversion.pri) {
    error("Could not find the file qtversion.pri")
}

!minQtVersion(4, 6, 0) {
    message("Cannot build OpenCAN with Qt version $${QT_VERSION}.")
    error("Qt 4.6 or greater is required.")
}

!include(qtperf.pri) {
    error("Could not find the file qtperf.pri")
}

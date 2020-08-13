# QMake configurations for enabling C++11 and fast QString concatenations

!isEmpty(QTPERF_PRI_INCLUDED): error("qtperf.pri already included")
QTPERF_PRI_INCLUDED = 1

# Enable C++11
isEqual(QT_MAJOR_VERSION, 5) {
    CONFIG += c++11
    QMAKE_CXXFLAGS += -std=c++11
} else {
    macx {
        !macx-clang*: error("You need to use the macx-clang or macx-clang-libc++ mkspec to compile OpenCAN (call qmake with '-spec unsupported/macx-clang')")
        QMAKE_CFLAGS += -mmacosx-version-min=10.7
        QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
        QMAKE_OBJECTIVE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
        QMAKE_LFLAGS += -stdlib=libc++ -mmacosx-version-min=10.7
    } else:linux-g++* {
        QMAKE_CXXFLAGS += -std=c++0x
    } else:linux-icc* {
        QMAKE_CXXFLAGS += -std=c++11
    } else:linux-clang* {
        QMAKE_CXXFLAGS += -std=c++11
        QMAKE_LFLAGS += -stdlib=libc++ -lc++abi
    } else:win32-g++* {
        QMAKE_CXXFLAGS += -std=c++0x
    }
    # nothing to do for MSVC10+
}

# Disable automatic string conversions
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY

# Fast QString concatenations
lessThan(QT_VERSION, 4.8): DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS
greaterThan(QT_VERSION, 4.7): DEFINES *= QT_USE_QSTRINGBUILDER

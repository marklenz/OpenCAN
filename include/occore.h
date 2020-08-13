/******************************************************************************
**
**  Copyright (C) 2005-2020 Code of Intelligence, LLC.
**  Contact: contact@opencan.org
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public
**  License version 2.1 as published by the Free Software Foundation and
**  appearing in the file LICENSE included in the packaging of this file.
**  Please review the following information to ensure the GNU Lesser General
**  Public License version 2.1 requirements will be met:
**  http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**  Commercial License Usage
**  Licensees holding valid commercial OpenCAN licenses may use this file in
**  accordance with the commercial license agreement or other written agreement
**  between you and Code of Intelligence that governs your use of the Software.
**  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#ifndef OCCORE_H
#define OCCORE_H

#include <QtCore/QtGlobal>
#include <QDebug>

#define OPENCAN_VERSION_STR   "0.8"
/*
   OPENCAN_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define OPENCAN_VERSION 0x000800
/*
   can be used like #if (OPENCAN_VERSION >= OPENCAN_VERSION_CHECK(0, 6, 0))
*/
#define OPENCAN_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

// Use the preprocessor to set the correct direction of library import/export.
// OPENCAN_LIBRARY is added to DEFINES in the OpenCAN library QMake file.
/*#if defined(Q_OS_WIN) && defined(Q_CC_MINGW)
	#if defined(OPENCAN_LIBRARY)
		#define OPENCAN_EXPORT __attribute__((visibility("default")))
	#else
		#define OPENCAN_EXPORT
	#endif
#else*/
	#if defined(OPENCAN_LIBRARY)
		#define OPENCAN_EXPORT Q_DECL_EXPORT
	#else
		#define OPENCAN_EXPORT Q_DECL_IMPORT
	#endif
//#endif

#if defined(Q_OS_MAC) && !defined(Q_CC_INTEL)
    #define OPENCAN_HEADER_BEGIN extern "C++" {
    #define OPENCAN_HEADER_END }
#else
    #define OPENCAN_HEADER_BEGIN
    #define OPENCAN_HEADER_END
#endif

#ifndef QT5
// The following is the official fallback for QStringLiteral, see qstring.h
// in Qt 5 after #ifndef QStringLiteral
#define QStringLiteral(str) QString::fromUtf8("" str "", sizeof(str) - 1)
#endif

namespace OpenCAN {

// Function/method status return values
enum class Result : quint8 {
    Failure,
    Success,
    Empty,
    NotApplicable
};

// "Built-in" baud rates matching those offered by PCAN-Light and PCAN-Basic
enum class BaudRate {
    Baud_1M =    1000000,  //   1 MBit/s
    Baud_500K =  500000,  // 500 kBit/s
    Baud_250K =  250000,  // 250 kBit/s
    Baud_125K =  125000,  // 125 kBit/s
    Baud_100K =  100000,  // 100 kBit/s
    Baud_50K  =  50000,  //  50 kBit/s
    Baud_20K  =  20000,  //  20 kBit/s
    Baud_10K  =  10000,  //  10 kBit/s
    Baud_5K   =  5000,  //   5 kBit/s
};

} // namespace OpenCAN

// OpenCAN version as a QString
extern OPENCAN_EXPORT QString OpenCAN_Version();

// OpenCAN "about text"
extern OPENCAN_EXPORT QString OpenCAN_About();

#endif // OCCORE_H

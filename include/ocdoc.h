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

#ifndef OCDOC_H
#define OCDOC_H

////////////////////////////////////////////////////////////////////////////////
//
// This file contains the first page of the OpenCAN API Doxygen documentation
//
////////////////////////////////////////////////////////////////////////////////

/**		\mainpage

  \section Introduction

  The following is documentation for compiling, installing and using the OpenCAN API.

  \section Compiling

  OpenCAN is distributed as five separate projects, each with their own QMake \a .pro file.
  <table>
  <tr><td>OpenCAN.pro</td><td>OpenCAN library</tr>
  <tr><td>ocplugins/SimCAN.pro</td><td>SimCAN device plugin <em>(All platforms)</em></tr>
  <tr><td>ocplugins/PCAN.pro</td><td>PCAN device plugin <em>(Windows and Linux)</em></tr>
  <tr><td>ocplugins/LinCAN.pro</td><td>LinCAN device plugin <em>(Linux only)</em></tr>
  <tr><td>Testbench/Testbench.pro</td><td>Testbench application</tr>
  </table>
  Only the OpenCAN library is compiled using the OpenCAN.pro project file, and it must be
  compiled first.
  The other projects are optional, but you will need an OpenCAN device plugin to use the
  CAN bus or the SimCAN device plugin for the simulated CAN bus.

  OpenCAN has been tested on the following configurations:
  <ul>
    <li>Microsoft Windows 7 (32-bit) with Visual Studio 2010 SP1 or MinGW gcc 4.8.0</li>
    <li>Microsoft Windows 8.1 (32-bit and 64-bit) with Visual Studio 2012 SP2</li>
    <li>Apple Mac OS X 10.9 "Mavericks", Cocoa (64-bit) with the Apple-provided Clang</li>
    <li>Linux kernel 3.0+ with GCC 4.6+ and X.org 7.6+</li>
  </ul>

  To compile and run the PCAN or LinCAN device plugin, the device driver and its library
  need to be installed first.

  Qt Creator is the recommended tool to compile for users new to the Qt library. Qt Creator
  can be used to compile and debug each project by opening the project's .pro file, or you
  can follow the platform-specific compilation instructions using QMake. Qt Creator invokes
  QMake for you and is often the easiest way to get started with OpenCAN.
  - \subpage compilewin
  - \subpage compilelin
  - \subpage compilemac

  \section Installing

  Currently, there is no installation method. Simply copy the compiled files to the
  desired location or leave them where they are. However, adding the OpenCAN include
  and library directories to the system include and library paths can simplify
  compilation of plugins and software using the OpenCAN library.

  Compiling the PCAN device plugin requires the latest version of the PCAN-Light API
  from PEAK-System. Unless the PCAN-Light include and library directories are added
  to the

  \section Using

  To use the OpenCAN API in another project, you must add the OpenCAN include and
  library directories to your system include and library paths or set the flags
  appropriately in the project's QMake file. The Testbench QMake .pro file may be a
  helpful example of how to set the directories.

  To use OpenCAN Testbench simply run the executable. Testbench does very little
  without an OpenCAN device plugin installed and connected to a device.

  \section Troubleshooting

  \subsection Loading Plugins

  If you are having trouble loading the PCAN plugin in Windows, you may need to
  copy the PCAN_USB.dll from the PEAK software into the OpenCAN folder with
  testbench.exe.

  Other reasons for a plugin not loading is if the plugin is built with a different
  version of Qt than the Qt used to build OpenCAN, or if the plugin is not built with the
  installed version of OpenCAN loaded at runtime.
**/

/** \page compilewin Compile for Windows
  \section vs Compile using Visual Studio
  You must have the Qt library built for a supported version of Visual Studio
  installed before compiling and QTDIR\bin must be in your PATH. This is done
  for you if you use the Qt installer.
  - Open a Visual Studio Command Prompt (64-bit if compiling for 64-bit)
  - Execute \a qmake -tp vc to create the Visual Studio project
  - Use Visual Studio to compile and debug the project

  \section MinGW Compile using mingw
  You must have the Qt library built for MinGW installed before compiling.
  - Open a Command Prompt
  - Change to the directory containing the QMake file for the project to compile.
  - Execute \a qmake to create the gmake Makefile
  - Execute \a mingw32-make
**/

/** \page compilelin Compile for Linux
  - Change to the directory containing the QMake file for the project to compile.
  - Execute \a qmake to create the gmake Makefile.
  - Execute \a make to build the project
**/

/** \page compilemac Compile for Mac OS X
  - Change to the directory containing the QMake file for the project to compile.
  - Execute \a qmake to create the XCode project.
  - Open the created XCode project to compile and debug
**/

#endif // OCDOC_H

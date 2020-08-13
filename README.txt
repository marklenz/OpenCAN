OpenCAN v0.8 - Open software platform for controlling CAN devices

Welcome to the OpenCAN community!

== What is OpenCAN? ==
OpenCAN is an open source platform for building Windows, Mac and Linux software that can interact with various Controller Area Network (CAN) buses and devices. It provides an abstract C++ interface that can be used to control the basic functions of any CAN device. Support for specific devices are written as OpenCAN plugins and loaded through a simple API call. So the hardware CAN drivers can be accessed through a generic interface, and application code can request a specific driver or use any that are available. Each component is cross-platform, enabling the efficient development of CAN software on Windows, Mac OS X and Linux. Everyone is free to use and extend OpenCAN according to the GNU Lesser General Public License (LGPL).


== Benefits ==
Simplifies adding CAN communication to software
Write one set of code that supports multiple devices and OSes
Add, remove or change CAN devices/drivers without modifying application code or recompiling
Simulate CAN devices using a simple C++ API instead of proprietary code
Reduces development and testing time by providing a simulated CAN bus to develop and test application code
Quickly prototype, test and demo software without device hardware
Reduces workspace clutter - no wires needed for simulation
Enables a greater separation between networking and application code
Minimizes hardware-specific code to a few simple methods
Reduces maintenance time and cost
Enables backward and forward compatibility with CAN devices, CAN driver APIs, OSes and hardware



== Features ==
Supports Windows, Mac OS X and Linux
Light-weight and simple API encapsulation of hardware CAN drivers
Fully documented API using Doxygen
Specific CAN hardware support can be statically compiled or loaded at runtime through plugins
Includes plugins for PEAK-System USB-PCAN devices on Windows and Linux as well as for Linux LinCAN devices
Includes the SimCAN device plugin for simulating a CAN bus across multiple applications
Includes a cross-platform GUI CAN bus analyzer and simulator called Testbench


== SimCAN Device Plugin ==
The cross-platform open source SimCAN device plugin uses an Inter-Process Communication (IPC) shared memory buffer to simulate a physical CAN bus that multiple applications can use to send and receive CAN messages between each other as if they were on a physical CAN bus. It can be used to develop and test application code before the hardware is ready or for fairly light-weight and simple communication between desktop applications. When the hardware arrives, simply replace the SimCAN plugin with a plugin for the physical CAN hardware. Since OpenCAN provides a generic interface to hardware CAN drivers, the applications will communicate over the physical CAN bus unmodified.

Applications using the SimCAN device plugin can be started and begin using the device in any order. The first application to start the simulated device will setup the IPC shared memory buffer, and other applications will simply begin using the buffer when they start the device. Similarly, the simulated CAN bus is not affected by the timing or order of an application stopping the device, and the shared memory buffer is not destroyed until all applications using it have stopped or closed.

The simulated CAN bus is not part of the core OpenCAN library. Every application that intends on communicating over the simulated CAN bus must use the OpenCAN library and the SimCAN device plugin. It is not impossible to use the IPC shared memory buffer without SimCAN, but to ensure interoperability and proper function across software developed by different people, it is strongly recommended against.

While the SimCAN device plugin simulates a physical CAN bus from the perspective of the application code that interacts with it, SimCAN does not try to simulate real-world conditions such as noise, physical limitations on transfer speed, power failures, etc. So an application that runs properly using the SimCAN plugin could fail to perform as expected when connected to a physical CAN bus, if the same ideal conditions are expected for the physical network.

The SimCAN device plugin has been used to simulate an entire network of up to 16 applications each sending a CAN message about once every second. The upper bound on devices and bandwidth is system-dependent, and the upper bounds are likely much higher than the those for a physical CAN bus.

The SimCAN device plugin is intended for desktop software use (Windows, Mac and Linux) and not for use on systems with contrained resources, but you are free to try if you want to.


== Testbench CAN Bus Analyzer and Simulator ==
Testbench is a cross-platform open source CAN bus analyzer and simulator for one or more networks. Testbench provides a test and simulation framework for software that incorporates CAN communication and serves as a source code example of using OpenCAN in an application. It can be used to test CAN drivers in development or interact with other applications using the SimCAN device plugin. When using the SimCAN device plugin, Testbench can be used to monitor and send CAN messages to other applications as if they were all on a network connected by a physical CAN bus. So Testbench can be a useful tool from the start of development through the entire life of the software.

Testbench is not required to use the SimCAN device plugin. Any two or more applications using the plugin can communicate via the simulated CAN bus.


== Requirements ==
Qt 4.6+/5.0+ (http://qt.nokia.com)
   The OpenCAN library and device plugins only require Qt's QtCore component
   Testbench requires the additional QtWidgets component for Qt5 or the QtGui component for Qt4

To compile and run the PCAN or LinCAN device plugins, the device driver and its library need to be installed first.

The PCAN device plugin requires the PCAN-Light API
    Windows: http://www.peak-system.com/Software-APIs.305.0.html?&L=1
        PCAN-USB package including the device driver and the PCAN-Basic API: http://www.peak-system.com/PCAN-USB.199.0.html?&L=1
        Create folder C:\PEAK
        Place the extracted folders and files Drivers, PCAN-Basic API, etc. into C:\PEAK
        Install the PCAN-USB driver according to the accompanying installation instructions
        Place the PCAN-Light API folder in C:\PEAK
        Copy the PCAN_USB.dll to the Testbench directory or add to library path
    Linux: http://www.peak-system.com/fileadmin/media/linux/index.htm
        Compile and install kernel driver and PCAN library according to the accompanying installation instructions


== Operating System and Compiler Support ==
Microsoft Windows 7 (32-bit) with Visual Studio 2010 SP1 or MinGW gcc 4.8.0
Microsoft Windows 8.1 (32-bit and 64-bit) with Visual Studio 2012 SP2
Apple Mac OS X 10.9 "Mavericks", Cocoa (64-bit) with the Apple-provided Clang
Linux kernel 3.0+ with GCC 4.6+ and X.org 7.6+

For Windows, at least Visual Studio 2010 SP 1 or MinGW gcc 4.8.0 must be used for C++11 support.

These are a subset of the Qt Reference Configurations for the latest Qt version at the time this version of OpenCAN is released (Qt 5.3). While OpenCAN should compile and run properly on any platform that a supported version of Qt does, the above OS and compiler combinations are the only combinations verified to run properly. See the Qt documentation for more information: http://qt-project.org/doc/qt-5/supported-platforms.html.

The Linux distributions that are used to test OpenCAN are Red Hat Enterprise Linux 6 and Fedora, but any Linux installation with the same versions of kernel, GCC and X.org or newer than what is provided in Ubuntu 11.10 should compile and run properly. See the Qt documentation for more information: http://qt-project.org/doc/qt-5/linux.html.


== Conditions of Use ==
The OpenCAN library as well as the LinCAN and PCAN device plugins are distributed under the GNU Lesser General Public License (LGPL) version 2.1 and may be used under version 2.1 of the LGPL or any later version.

Testbench and the SimCAN device plugin are distributed under the GNU General Public License (GPL) version 3.0 and may be used under version 3.0 of the GPL or any later version.

Commercial licenses can be provided for any or all of the software distributed in this OpenCAN package. Please contact us at sales@codeofintelligence.com for more information.

The OpenCAN library, the device plugins and Testbench are provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


== Build ==
Projects are built using Qt QMake and a C++ compiler.

Specific build instructions can be found at docs/index.html


== Install ==
Currently, there is no installation method. Simply copy the compiled files to the desired location or leave them where they are. See the OpenCAN documentation for further details on installation and use.


Using OpenCAN in your projects? We'd like to hear from you. Please tell us what you do and don't like as well as what you'd like to see in the future by sending an email to feedback@opencan.org. We appreciate any help in making OpenCAN more useful to the community.

Thank you for using OpenCAN!



Copyright (C) 2005-2014 Code of Intelligence, LLC.
(http://www.codeofintelligence.com)
Contact: contact@opencan.org

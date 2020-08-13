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

#include <ocdevice.h>

using namespace OpenCAN;

static const int AUTON_TIMER_MSECS = 1;
//static const int AUTON_TIMER_MSECS = 0;   // signal any time there are no window/system events to process

/// Create a CAN device
/// @param id the device identifier specific to the device type
OcDevice::OcDevice(const QString &id)
{
    devId = id;
    autonomous = false;
	started = false;
    baudRate = 0;
    autonTimerId = 0;
    extendedCAN = true;
    loopback = false;
}

/// OcDevice destructor
///
/// Subclasses should implement a destructor and call stop() to ensure
/// that hardware is properly uninitialized. Since stop() calls the
/// subclass's uninitialize(), stop() can not be called from the
/// superclass destructor.
OcDevice::~OcDevice()
{

}

/// Set the device to run autonomously.
/// This should be set before starting the device, otherwise it will have no
/// effect until the device is shutdown and started again.
///
/// If the device is set to run autonomously, every millisecond the plugin
/// will check if the device has received a new message whenever the device
/// has been started. If a message has been received, the messageReceived()
/// signal will be emitted.
/// @param autonomous true to run autonomously, default is false
/// @returns nothing
void OcDevice::setAutonomous(bool goAutonomous)
{
    autonomous = goAutonomous;
}

/// Initialize the device.
/// This method also starts the autonomous process.
/// @returns OcSuccess if the device wasn't already initialized and is then properly
/// initialized
Result OcDevice::start()
{
    if (started)
        return Result::NotApplicable;

    if (initialize() == Result::Success)
    {
        if (autonomous)
            autonTimerId = startTimer(AUTON_TIMER_MSECS);

        return Result::Success;
    }

    return Result::Failure;
}

/// Uninitialize the device.
/// Implement this method in device-specific subclasses and then call this
/// method, returning its value. This method stops the autonomous process.
/// @returns OcSuccess if was initialized and then properly uninitialized
Result OcDevice::stop()
{
    if (!started)
        return Result::NotApplicable;

    if (autonomous)
        killTimer(autonTimerId);

    return uninitialize();
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcDevice::sendMessage(OcMessage &msg)
{
    Q_ASSERT(started);

    // only implement loopback for autonomous mode
    if (loopback && autonomous)
    {
        msg.setTimestamp(QTime::currentTime());
        emit messageReceived(msg);
    }

    return internalSendMessage(msg);
}

// For autonomous mode

void OcDevice::timerEvent(QTimerEvent *)
{
    // emit signal for each waiting received message
    while (getMessage(autonMsg) == Result::Success)
        emit messageReceived(autonMsg);
}

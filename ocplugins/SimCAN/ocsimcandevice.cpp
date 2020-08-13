/******************************************************************************
**
**  Copyright (C) 2005-2020 Code of Intelligence, LLC.
**  Contact: contact@opencan.org
**
**  GNU General Public License Usage
**  This file may be used under the terms of the GNU General Public License
**  version 3.0 as published by the Free Software Foundation and appearing
**  in the file LICENSE included in the packaging of this file.  Please review
**  the following information to ensure the GNU General Public License version
**  3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
**  Commercial License Usage
**  Licensees holding valid commercial OpenCAN and SimCAN licenses may use
**  this file in accordance with the commercial license agreement or other
**  written agreement between you and Code of Intelligence that governs your
**  use of the Software.  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#include "ocsimcandevice.h"

#include  <QDebug>

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */

static const QString SIMCAN_DEVICE_TEXT = QStringLiteral("SimCAN ");

OcSimCANDevice::OcSimCANDevice(const QString &deviceNum) :
    OcDevice(deviceNum), buffer(deviceNum)
{
    setDeviceText(SIMCAN_DEVICE_TEXT + deviceNum);
}

/// Free memory and uninitialize the device
OcSimCANDevice::~OcSimCANDevice()
{
    stop();
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcSimCANDevice::internalSendMessage(const OcMessage &msg)
{
    Message ipcMsg;
    ipcMsg.id = msg.id() | (msg.extended() ? CAN_EFF_FLAG : 0);
    ipcMsg.dataLength = msg.length();
    std::copy(msg.data(), &msg.data()[msg.length()], ipcMsg.data);

    Result retval = Result::Success;
    if (!buffer.write(ipcMsg))
    {
        qDebug() << QStringLiteral("OcSimCANDevice::internalSendMessage(): Unable to write message to shared memory");
        retval = Result::Failure;
    }

    return retval;
}

/// Get a received message.
/// The device must be started before calling this method.
/// The OcMessage object will be filled with the CAN message data including
/// a timestamp of when the message was received.
/// @param msg the message to fill with new data
/// @returns OcSuccess if a new message was successfully copied into msg,
/// OcEmpty if there are no messages in the hardware buffer, or another
/// value upon error.
Result OcSimCANDevice::getMessage(OcMessage &msg)
{
    Q_ASSERT(started);

    Message ipcMsg;
    if (buffer.read(ipcMsg))
    {
        msg.setExtended((ipcMsg.id & CAN_EFF_FLAG) == CAN_EFF_FLAG);
        msg.setId(ipcMsg.id & (msg.extended() ? CAN_EFF_MASK : CAN_SFF_MASK));
        msg.setTimestamp(QTime::currentTime());
        msg.setData(ipcMsg.data);
        return Result::Success;
    }
    else
    {
        return Result::Empty;
    }
}

/// Initialize the device.
/// @returns OcSuccess if the device was not already initialized and has been
/// successfully initialized.
Result OcSimCANDevice::initialize()
{
    Result retval = Result::Success;

    if (buffer.open())
    {
        started = true;
    }
    else
    {
        qDebug() << QStringLiteral("OcSimCANDevice::initialize(): Unable to open shared memory");
        retval = Result::Failure;
    }

    return retval;
}

/// Uninitialize the device.
/// @returns OcSuccess if the device was initialized and has been successfully
/// uninitialized.
Result OcSimCANDevice::uninitialize()
{
    Result retval = Result::Success;

    if (buffer.close())
    {
        started = false;
    }
    else
    {
        qDebug() << QStringLiteral("OcSimCANDevice::uninitialize(): Unable to close shared memory");
        retval = Result::Failure;
    }

    return retval;
}

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

#include "ocsocketcandevice.h"

#include <linux/can/raw.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#include <linux/can/netlink.h>
#include <linux/can.h>

static const QString DEV_PREFIX = QStringLiteral("can");
static const int DEFAULT_BAUD_RATE = 250000;			// 250000 = 250kbps
static const int CAN_ERROR = -1;

OcSocketCanDevice::OcSocketCanDevice(const QString &deviceId) :
    OcDevice(deviceId)
{
    sock = 0;
    baudRate = DEFAULT_BAUD_RATE;
    devFile = DEV_PREFIX + deviceId;

    setDeviceText(DEV_PREFIX + deviceId);
}

OcSocketCanDevice::~OcSocketCanDevice()
{
    stop();
}

/// Set the device baud rate
/// @param baud the baud rate in bits per second
/// @returns OcSuccess if successful
Result OcSocketCanDevice::setBaudRate(qint32 baud)
{
    Q_ASSERT(!started);

    baudRate = baud;

    struct can_bittiming bt;
    memset(&bt, 0, sizeof(bt));

    bt.bitrate = baud;
    bt.sjw = 0;             // Synchronization Jump Width: range 0-3
    bt.sample_point=0.875;	// 87.5%
    if (can_set_bittiming(devFile.toLatin1().constData(), &bt) != 0)
    {
        qDebug() << QStringLiteral("OcSocketCanDevice::setBaudRate(): can_set_bittiming error %1").arg(errno);
        return Result::Failure;
    }
    else
        return Result::Success;
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcSocketCanDevice::internalSendMessage(const OcMessage &msg)
{
    Q_ASSERT(started);

    struct can_frame frame;
    frame.can_id = msg.id() | (msg.extended() ? CAN_EFF_FLAG : 0);
    frame.can_dlc = msg.length();
    std::copy(msg.data(), &msg.data()[frame.can_dlc], frame.data);

    Result retval = Result::Success;
    int bytesWritten = 0;
    do
    {
        bytesWritten = write(sock, &frame, sizeof(frame));
        if (bytesWritten != sizeof(frame))		// Write error
        {
            qDebug() << QStringLiteral("OcSocketCanDevice::internalSendMessage(): CAN write error %1").arg(errno);
            retval = Result::Failure;
        }
    } while (bytesWritten == 0);

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
Result OcSocketCanDevice::getMessage(OcMessage &msg)
{
    Q_ASSERT(started);

    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));

    int ret = read(sock, &frame, sizeof(frame));
    if (ret < 0)
    {
        qDebug() << QStringLiteral("OcSocketCanDevice::getMessage(): CAN read error %1").arg(errno);
        return Result::Failure;
    }
    else
    {
        msg.setExtended((frame.can_id & CAN_EFF_FLAG) == CAN_EFF_FLAG);
        msg.setId(frame.can_id & (msg.extended() ? CAN_EFF_MASK : CAN_SFF_MASK));
        msg.setTimestamp(QTime::currentTime());
        msg.setData(frame.data, frame.can_dlc);
        return Result::Success;
    }
    return Result::Success;
}

/// Initialize the device.
/// @returns OcSuccess if the device was not already initialized and has been
/// successfully initialized.
Result OcSocketCanDevice::initialize()
{
    Result retval = Result::Success;

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Locate the interface you wish to use
    struct ifreq ifr;
    strcpy(ifr.ifr_name, devFile.toLocal8Bit().constData());
    ioctl(sock, SIOCGIFINDEX, &ifr); // ifr.ifr_ifindex gets filled with the selected device's index

    // Select that CAN interface, and bind the socket to it
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (sock > 0)
    {
        started = true;
    }
    else
    {
        qDebug() << QStringLiteral("OcSocketCanDevice::initialize(): CAN open error %1").arg(errno);
        retval = Result::Failure;
    }

    return retval;
}

/// Uninitialize the device.
/// @returns OcSuccess if the device was initialized and has been successfully
/// uninitialized.
Result OcSocketCanDevice::uninitialize()
{
    Result retval = Result::Success;

    if (close(sock) != CAN_ERROR)
    {
        started = false;
    }
    else
    {
        qDebug() << QStringLiteral("OcSocketCanDevice::uninitialize(): CAN close error %1").arg(errno);
        retval = Result::Failure;
    }

    return retval;
}

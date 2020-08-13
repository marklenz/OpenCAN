/******************************************************************************
**
**  Copyright (C) 2005-2020 Code of Intelligence, LLC.
**  Contact: contact@opencan.org
**
**  This file may be used under the terms of the GNU Lesser General Public
**  License version 2.1 as published by the Free Software Foundation and
**  appearing in the file LICENSE included in the packaging of this file.
**  Please review the following information to ensure the GNU Lesser General
**  Public License version 2.1 requirements will be met:
**  http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
******************************************************************************/

#include "oclincandevice.h"

#include "lincan.h"

#include <fcntl.h>
#include <unistd.h>

static const QString DEV_PREFIX = QStringLiteral("/dev/can");
static const int BAUD_RATE = 250000;			// 250000 = 250kbps
static const int CAN_ERROR = -1;

static const int USECS_IN_MSECS = 1000;

OcLinCanDevice::OcLinCanDevice(const QString &deviceId) :
    OcDevice(deviceId)
{
	fd = 0;
	baudRate = BAUD_RATE;
    devFile = DEV_PREFIX + deviceId;

    setDeviceText(DEV_PREFIX + deviceId);
}

/// Free memory and uninitialize the device
OcLinCanDevice::~OcLinCanDevice()
{
    stop();
}

/// Set the device baud rate
/// @param baud the baud rate in bits per second
/// @returns OcSuccess if successful
Result OcLinCanDevice::setBaudRate(qint32 baud)
{
    baudRate = baud;

    if (!started)
        return Result::Success;

    can_baudparams_t lcparams;

    lcparams.flags = 0;			// bus sample rate: BTR1_SAM=three times, 0=once
    lcparams.baudrate = baud;
    lcparams.sjw = 0;			// Synchronization Jump Width: range 0-3
    lcparams.sample_pt = 8750;	// 87.5%

    int ret = ioctl(fd, CONF_BAUDPARAMS, &lcparams);
    if (ret < 0)
    {
        qDebug() << QStringLiteral("OcLinCanDevice::setBaudRate(): ioctl error %1").arg(errno);
        perror("ioctl bitrate error");
        return Result::Failure;
    }
    else
        return Result::Success;
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcLinCanDevice::internalSendMessage(const OcMessage &msg)
{
    Q_ASSERT(started);

	// LinCAN message structure
	canmsg_t lcmsg;
	lcmsg.cob = 0; 					// CAN object number, used in Full CAN
	lcmsg.timestamp.tv_sec = 0;
    lcmsg.timestamp.tv_usec = 0;

	if (msg.extended())
		lcmsg.flags = MSG_EXT;
	lcmsg.id = msg.id();
	lcmsg.length = msg.length();

    std::copy(msg.data(), &msg.data()[msg.length()], lcmsg.data);

    Result retval = Result::Success;
	int bytesWritten = 0;
	do
	{
		bytesWritten = write(fd, &lcmsg, sizeof(canmsg_t) - (MAX_MESSAGE_LENGTH - msg.length()));
		if (bytesWritten < 0)		// Write error
		{
            qDebug() << QStringLiteral("OcLinCanDevice::internalSendMessage(): CAN write error %1").arg(errno);
            retval = Result::Failure;
		}
	} while (bytesWritten == 0);	// CAN is timing out

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
Result OcLinCanDevice::getMessage(OcMessage &msg)
{
    Q_ASSERT(started);

	canmsg_t lcmsg;
	memset(&lcmsg, 0, sizeof(canmsg_t));

	int ret = read(fd, &lcmsg, sizeof(canmsg_t));
	if (ret < 0)
	{
        qDebug() << QStringLiteral("OcLinCanDevice::getMessage(): CAN read error %1").arg(errno);
		perror("read error");
        return Result::Failure;
	}
    else if (ret == 0)
    {
        // no message in receive buffer
        return Result::Empty;
    }
	else
	{
		bool ext = (lcmsg.flags & MSG_EXT) == MSG_EXT;
        msg.setExtended(ext);
        msg.setId(lcmsg.id);
        msg.setTimestamp(QTime::currentTime());
        msg.setData(lcmsg.data, lcmsg.length);
        return Result::Success;
	}
}

/// Initialize the device.
/// @returns OcSuccess if the device was not already initialized and has been
/// successfully initialized.
Result OcLinCanDevice::initialize()
{
    Result retval = Result::Success;

    fd = open(devFile.toLatin1().constData(), O_RDWR);
    if (fd != CAN_ERROR)		// CAN successfully opened
    {
        started = true;
        setBaudRate(baudRate);
    }
    else
    {
        qDebug() << QStringLiteral("OcLinCanDevice::initialize(): CAN open error %1").arg(errno);
        retval = Result::Failure;
    }

    return retval;
}

/// Uninitialize the device.
/// @returns OcSuccess if the device was initialized and has been successfully
/// uninitialized.
Result OcLinCanDevice::uninitialize()
{
    Result retval = Result::Success;

    if (close(fd) != CAN_ERROR)
    {
        started = false;
    }
    else
    {
        qDebug() << QStringLiteral("OcLinCanDevice::uninitialize(): CAN close error %1").arg(errno);
        retval = Result::Failure;
    }

    return retval;
}

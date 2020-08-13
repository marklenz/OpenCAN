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

#include "ocpcanlightdevice.h"

#if defined(Q_OS_WIN)
	#include <windows.h>
    #include <pcan_usb.h>
#else
    #include <fcntl.h>
	#include <libpcan.h>
#endif

static const QString CAN_DEV = QStringLiteral("/dev/pcan");			// Device filename
static const int INIT_BAUD_RATE = CAN_BAUD_250K;	// 250kbps

static const QString USB_DEVICE_ID = QStringLiteral("32");
static const QString USB_DEVICE_TEXT = QStringLiteral("PCAN-Light USB");

OcPcanLightDevice::OcPcanLightDevice(const QString &deviceId) :
    OcDevice(deviceId)
{
    baudRate = INIT_BAUD_RATE;
    prevTimestamp = 0;

    // Windows PCAN-Light does not use device names
#if defined(Q_OS_UNIX)
    fd = 0;
    devFile = CAN_DEV + deviceId;

    if (deviceId == USB_DEVICE_ID)
#endif
        setDeviceText(USB_DEVICE_TEXT);

    bauds.append((qint32)BaudRate::Baud_1M);	//   1 MBit/s
    bauds.append((qint32)BaudRate::Baud_500K);	// 500 kBit/s
    bauds.append((qint32)BaudRate::Baud_250K);	// 250 kBit/s
    bauds.append((qint32)BaudRate::Baud_125K);	// 125 kBit/s
    bauds.append((qint32)BaudRate::Baud_100K);	// 100 kBit/s
    bauds.append((qint32)BaudRate::Baud_50K);	//  50 kBit/s
    bauds.append((qint32)BaudRate::Baud_20K);	//  20 kBit/s
    bauds.append((qint32)BaudRate::Baud_10K);	//  10 kBit/s
    bauds.append((qint32)BaudRate::Baud_5K);	//   5 kBit/s
}

/// Free memory and uninitialize the device
OcPcanLightDevice::~OcPcanLightDevice()
{
    stop();
}

/// Set the device baud rate
/// @param baud the baud rate in bits per second
/// @returns OcSuccess if successful
Result OcPcanLightDevice::setBaudRate(qint32 baud)
{
#if defined(Q_OS_WIN)
    // can't reset baud rate with Windows driver
    if (started)
        return Result::Failure;
    else
    {
        baudRate = baud;
        return Result::Success;
    }
#else
    baudRate = baud;
    return setBaudRateLinux();
#endif
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcPcanLightDevice::internalSendMessage(const OcMessage &msg)
{
    Q_ASSERT(started);

    // PCAN-Light message structure
	TPCANMsg pcmsg;

	if (msg.extended())
		pcmsg.MSGTYPE = MSGTYPE_EXTENDED;
	else
		pcmsg.MSGTYPE = MSGTYPE_STANDARD;
	pcmsg.ID = msg.id();
	pcmsg.LEN = msg.length();

    std::copy(msg.data(), &msg.data()[msg.length()], pcmsg.DATA);

    Result retval = Result::Success;
    int err = 0;
#if defined(Q_OS_WIN)
    err = CAN_Write(&pcmsg);
#else
    err = LINUX_CAN_Write_Timeout(fd, &pcmsg, 1000);
#endif

    if (err != CAN_ERR_OK)		// Write error
    {
        qDebug() << QStringLiteral("OcPcanLightDevice::internalSendMessage: CAN write error %1").arg(err);
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
Result OcPcanLightDevice::getMessage(OcMessage &msg)
{
    Q_ASSERT(started);

#if defined(Q_OS_WIN)
	TPCANMsg pcmsg;
	pcmsg.ID = 0;
	memset(pcmsg.DATA, 0, 8);
	pcmsg.LEN = 0;

	TPCANTimestamp time;

	DWORD ret = CAN_ReadEx(&pcmsg, &time);
    if (ret == CAN_ERR_OK)
    {
        msg.setId(pcmsg.ID);
        setTimestamp(time.millis);
        msg.setTimestamp(prevTime);
        msg.setData(pcmsg.DATA, pcmsg.LEN);
        msg.setExtended(pcmsg.MSGTYPE == MSGTYPE_EXTENDED);
        return OcSuccess;
    }
    else if (ret == CAN_ERR_QRCVEMPTY)
    {
        // no message in receive buffer
        return OcEmpty;
    }
    else
    {
        qDebug() << QStringLiteral("OcPcanLightDevice::getMessage: CAN read error %1").arg(ret);
        return OcFail;
    }
#else
	TPCANRdMsg pcmsg;
	pcmsg.Msg.ID = 0;
	memset(pcmsg.Msg.DATA, 0, 8);
	pcmsg.Msg.LEN = 0;

	int ret = LINUX_CAN_Read_Timeout(fd, &pcmsg, 0);
    if (ret == CAN_ERR_OK)
    {
        msg.setId(pcmsg.Msg.ID);
        setTimestamp(pcmsg.dwTime);
        msg.setTimestamp(prevTime);
        msg.setData(pcmsg.Msg.DATA, pcmsg.Msg.LEN);
        msg.setExtended(pcmsg.Msg.MSGTYPE == MSGTYPE_EXTENDED);
        return Result::Success;
    }
    else if (ret == CAN_ERR_QRCVEMPTY)
	{
        // no message in receive buffer
        return Result::Empty;
	}
    else
	{
        qDebug() << QStringLiteral("OcPcanLightDevice::getMessage: CAN read error %1").arg(ret);
        return Result::Failure;
	}
#endif
}

/// Initialize the device.
/// @returns OcSuccess if the device was not already initialized and has been
/// successfully initialized.
Result OcPcanLightDevice::initialize()
{
    Result retval = Result::Success;

#if defined(Q_OS_WIN)
    int err = CAN_Init(baudRate, isExtended());
    if (err == CAN_ERR_OK)
    {
        started = true;
    }
    else	// Couldn't open CAN
    {
        qDebug() << QStringLiteral("OcPcanLightDevice::initialize: CAN open error %1").arg(err);
        retval = OcFail;
    }
#else
    fd = LINUX_CAN_Open(devFile.toLatin1().constData(), O_RDWR);
    if (fd != nullptr)
    {
        started = true;
        setBaudRateLinux();
    }
    else	// Couldn't open CAN
    {
        qDebug() << QStringLiteral("OcPcanLightDevice::initialize: CAN open error %1").arg(errno);
        retval = Result::Failure;
    }
#endif

    return retval;
}

/// Uninitialize the device.
/// @returns OcSuccess if the device was initialized and has been successfully
/// uninitialized.
Result OcPcanLightDevice::uninitialize()
{
    // reset timestamp variables
    prevTime = QTime();
    prevTimestamp = 0;

#if defined(Q_OS_WIN)
    CAN_Close();
    started = false;
#else
    CAN_Close(fd);
    started = false;
#endif

    return Result::Success;
}

Result OcPcanLightDevice::setBaudRateLinux()
{
    if (!started)
        return Result::Success;

    int result = 0;
    if (isExtended())
        CAN_Init(fd, baudRate, CAN_INIT_TYPE_EX);
    else
        CAN_Init(fd, baudRate, CAN_INIT_TYPE_ST);

    if (result)
    {
        qDebug() << QStringLiteral("OcPcanLightDevice::setBaudRateLinux: error");
        return Result::Failure;
    }
    else
        return Result::Success;
}

/// Use the milliseconds returned from the driver to generate an appropriate
/// QTime and store in prevTime.
void OcPcanLightDevice::setTimestamp(quint32 msecs)
{
    if (prevTime.isNull())
        prevTime = prevTime.currentTime();
    else
    {
        if (msecs < prevTimestamp)
            // timestamp rolled over
            prevTime = prevTime.addMSecs((sizeof(prevTimestamp) * 0xFF) - prevTimestamp + msecs + 1);
        else
            prevTime = prevTime.addMSecs(msecs - prevTimestamp);
    }
    prevTimestamp = msecs;
}

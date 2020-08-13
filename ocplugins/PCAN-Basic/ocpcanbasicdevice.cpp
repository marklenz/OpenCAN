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

#include "ocpcanbasicdevice.h"

#if defined(Q_OS_WIN)
    #include <windows.h>
#else
    // provide Windows types in Linux needed for PCANBasic.h
    #include <asm/types.h>

    #define DWORD  __u32
    #define WORD   unsigned short
    #define BYTE   unsigned char
    #define LPSTR  char*

    #define linux   // needed in PCANBasic.h
#endif

#include <PCANBasic.h>

using namespace OpenCAN;

static const int INIT_BAUD_RATE = PCAN_BAUD_250K;	// 250kbps

static const QString USB_DEVICE_ID = QStringLiteral("32");
static const QString USB_DEVICE_TEXT = QStringLiteral("PCAN-Basic USB");

OcPcanBasicDevice::OcPcanBasicDevice(const QString &deviceId) :
    OcDevice(deviceId)
{
    baudRate = INIT_BAUD_RATE;
    prevTimestamp = 0;

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
OcPcanBasicDevice::~OcPcanBasicDevice()
{
    stop();
}

/// Set the device baud rate
/// @param baud the baud rate in bits per second
/// @returns OcSuccess if successful
Result OcPcanBasicDevice::setBaudRate(qint32 baud)
{
    // PCAN-Basic doesn't provide a means to set baud rate after initialization
    if (!started)
    {
        baudRate = baud;
        return Result::Success;
    }
    return Result::Failure;
}

/// Send a CAN message
/// The device must be started before calling this method.
/// @param msg the message to send
/// @returns OcSuccess if the message was successfully sent
Result OcPcanBasicDevice::internalSendMessage(const OcMessage &msg)
{
    Q_ASSERT(started);

    // PCAN-Basic message structure
    TPCANMsg pcmsg;

    if (msg.extended())
        pcmsg.MSGTYPE = PCAN_MESSAGE_EXTENDED;
    else
        pcmsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
    pcmsg.ID = msg.id();
    pcmsg.LEN = msg.length();

    std::copy(msg.data(), &msg.data()[msg.length()], pcmsg.DATA);

    Result retval = Result::Success;
    DWORD err = CAN_Write(PCAN_USBBUS1, &pcmsg);
    if (err != PCAN_ERROR_OK)
    {
        qDebug() << QStringLiteral("OcPcanBasicDevice::internalSendMessage: CAN write error %1").arg(err);
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
Result OcPcanBasicDevice::getMessage(OcMessage &msg)
{
    Q_ASSERT(started);

    TPCANMsg pcmsg;
    pcmsg.ID = 0;
    memset(pcmsg.DATA, 0, 8);
    pcmsg.LEN = 0;

    TPCANTimestamp time;

    DWORD ret = CAN_Read(PCAN_USBBUS1, &pcmsg, &time);
    if (ret == PCAN_ERROR_OK)
    {
        msg.setId(pcmsg.ID);
        setTimestamp(time.millis);
        msg.setTimestamp(prevTime);
        msg.setData(pcmsg.DATA, pcmsg.LEN);
        msg.setExtended(pcmsg.MSGTYPE == PCAN_MESSAGE_EXTENDED);
        return Result::Success;
    }
    else if (ret == PCAN_ERROR_QRCVEMPTY)
    {
        // no message in receive buffer
        return Result::Empty;
    }
    else
    {
        qDebug() << QStringLiteral("OcPcanBasicDevice::getMessage: CAN read error %1").arg(ret);
        return Result::Failure;
    }
}

/// Initialize the device.
/// @returns OcSuccess if the device was not already initialized and has been
/// successfully initialized.
Result OcPcanBasicDevice::initialize()
{
    Result retval = Result::Success;

    DWORD err = CAN_Initialize(PCAN_USBBUS1, baudRate, 0, 0, 0);
    if (err == PCAN_ERROR_OK)
    {
        started = true;
    }
    else	// Couldn't initialize CAN hardware
    {
        qDebug() << QStringLiteral("OcPcanBasicDevice::initialize: CAN open error %1").arg(err);
        retval = Result::Failure;
    }

    return retval;
}

/// Uninitialize the device.
/// @returns OcSuccess if the device was initialized and has been successfully
/// uninitialized.
Result OcPcanBasicDevice::uninitialize()
{
    // reset timestamp variables
    prevTime = QTime();
    prevTimestamp = 0;

    CAN_Uninitialize(PCAN_USBBUS1);
    started = false;

    return Result::Success;
}

/// Use the milliseconds returned from the driver to generate an appropriate
/// QTime and store in prevTime.
void OcPcanBasicDevice::setTimestamp(quint32 msecs)
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


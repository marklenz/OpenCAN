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

#ifndef OCDEVICE_H
#define OCDEVICE_H

#include <QObject>

#include <QList>
#include <QVarLengthArray>
#include <QTime>

#include <ocmessage.h>

OPENCAN_HEADER_BEGIN

/// The abstract base class for all OpenCAN CAN devices.

/// An OcDevice provides an abstract interface to a CAN device. It is
/// used to initialize/uninitialize the device as well as send and
/// receive CAN messages on the CAN bus the device is connected to.
///
/// Each OcDevice can be used in one of two receive modes: directed and
/// autonomous. The default mode is directed. The method getMessage()
/// must be called to get each message received on the CAN bus.
///
/// In autonomous mode, set by calling setAutonomous(), the plugin will
/// call getMessage() once every millisecond and emit the signal
/// messageReceived() when a message has been received. This reduces the
/// amount of code that needs to be written to receive CAN messages down
/// to one line connecting the signal to a slot to parse the message.
///
/// Each device must be initialized before using it to send or receive
/// CAN messages. Each device should also be uninitialized when no
/// longer needed to free memory and any hardware locks. Calls to
/// start() and stop() perform these actions.
///
/// Besides the SimCAN device, all devices can be set to communicate at
/// different baud rates using setBaudRate(). Depending upon the CAN
/// hardware and driver, the baud rate may not be changed once the
/// hardware is initialized. So, it is advised to set the baud rate
/// before starting the device or stopping the device before changing
/// the baud rate as a rule of thumb.
///
/// To make an OpenCAN device plugin, create a subclass of OcDevice
/// and implement the hardware-specific virtual methods. Then, create
/// a Qt plugin class as a sublcass of QObject and OcDeviceInterface.
class OPENCAN_EXPORT OcDevice : public QObject
{
	Q_OBJECT
public:
    explicit OcDevice(const QString &id);
	virtual ~OcDevice();

    // Abstract methods to be implemented by subclasses

    /// Set the device baud rate
    /// @param baud the baud rate in bits per second
    /// @returns OcSuccess if successful
    virtual OpenCAN::Result setBaudRate(qint32 baud) = 0;

    /// Get a received message.
    /// The device must be started before calling this method.
    /// The OcMessage object will be filled with the CAN message data including
    /// a timestamp of when the message was received.
    /// @param msg the message to fill with new data
    /// @returns OcSuccess if a new message was successfully copied into msg,
    /// OcEmpty if there are no messages in the hardware buffer, or another
    /// value upon error.
    virtual OpenCAN::Result getMessage(OcMessage &msg) = 0;

Q_SLOT
    virtual OpenCAN::Result sendMessage(OcMessage &msg);

    // Non-virtual methods

    QList<qint32> baudRates() const;

    qint32 getBaudRate() const;

    bool isAutonomous() const;
    void setAutonomous(bool autonomous);

    QString id() const;
    QString text() const;

    bool isLoopback() const;
    void setLoopback(bool isLoopback);

    bool isStarted() const;
    OpenCAN::Result start();
    OpenCAN::Result stop();

    bool isExtended();
    void setExtended(bool extended);

Q_SIGNALS:
	/// This signal is emitted when a message is received if
    /// isAutonomous() is true.
	/// If isAutonomous() is false, this signal is never emitted.
    void messageReceived(const OcMessage &);

protected:
    // Protected abstract methods to be implemented by subclasses

    /// Initialize the device.
    /// Implement this method in device-specific subclasses to initialize the
    /// CAN hardware.
    /// @returns OcSuccess if the device was not already initialized and has been
    /// successfully initialized.
    virtual OpenCAN::Result initialize() = 0;

    /// Uninitialize the device.
    /// Implement this method in device-specific subclasses to uninitialize the
    /// CAN hardware.
    /// @returns OcSuccess if the device was initialized and has been successfully
    /// uninitialized.
    virtual OpenCAN::Result uninitialize() = 0;

    /// Platform-specific code for sending CAN messages.
    /// Implement this method in device-specific subclasses to send a message.
    /// @param msg the message to send
    /// @returns OcSuccess if the message was successfully sent
    virtual OpenCAN::Result internalSendMessage(const OcMessage &msg) = 0;

    // Protected non-virtual methods and data members

	/// Set the device's text description
	/// @param deviceText the text to set as the device's description
	/// @returns nothing
	void setDeviceText(const QString &deviceText) { devTxt = deviceText; }

    bool loopback;
    bool started;
    qint32 baudRate;
    QList<qint32> bauds;

private:
	void timerEvent(QTimerEvent *);

	QString devId, devTxt;
	bool extendedCAN;

    // used for autonomous function
    bool autonomous;
    OcMessage autonMsg;
    int autonTimerId;
};

// Inline OcDevice Methods

/// Get the list of supported baud rates.
/// If the list is empty, the device supports an arbitrary
/// rate up to 1 Mbit/sec.
/// @returns a list of available baud rates
inline QList<qint32> OcDevice::baudRates() const { return bauds; }

/// Get the device's current baud rate
/// @returns the current baud rate
inline qint32 OcDevice::getBaudRate() const { return baudRate; }

/// Check if the device is set to run autonomously.
/// If autonomous, the device will poll the hardware for received
/// messages itself whenever started and emit the messageReceived()
/// signal when a message is received.
/// @returns true if the device is set to run autonomously
inline bool OcDevice::isAutonomous() const { return autonomous; }

/// Get the device's driver-specific identifier
inline QString OcDevice::id() const { return devId; }

/// Get the text description of the device
/// A human readable description of the device
inline QString OcDevice::text() const { return devTxt; }

/// Check if the device is in loopback mode
/// @returns OcSuccess if the device is in loopback mode
inline bool OcDevice::isLoopback() const { return loopback; }

/// Set the loopback mode which defaults to false
/// NOTE: Loopback mode only works when in autonomous mode, by emitting
/// messageReceived() when a message is sent using sendMessage().
/// @param isLoopback set true for loopback mode
inline void OcDevice::setLoopback(bool isLoopback) { loopback = isLoopback; }

/// Check if the device has been started
/// @returns OcSuccess if the device has been started
inline bool OcDevice::isStarted() const { return started; }

/// Query CAN 2.0b extended frame configuration
/// @returns true if device is configured for CAN 2.0b extended
inline bool OcDevice::isExtended() { return extendedCAN; }

/// Set CAN 2.0b extended (29-bit) or standard (11-bit) frame
/// @param extended set true to configure device for CAN 2.0b extended frame
/// @returns nothing
inline void OcDevice::setExtended(bool extended) { extendedCAN = extended; }

typedef QList<OcDevice *> OcDeviceList;

OPENCAN_HEADER_END

#endif // OCDEVICE_H

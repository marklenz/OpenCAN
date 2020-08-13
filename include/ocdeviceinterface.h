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

#ifndef OCDEVICEINTERFACE_H
#define OCDEVICEINTERFACE_H

#include <QObject>

#include <ocdevice.h>
#include <ocplugininfo.h>

OPENCAN_HEADER_BEGIN

#define OcDeviceInterface_iid "com.codeofintelligence.OpenCAN.OcDeviceInterface/0.8"

/// OpenCAN device plugin interface class.

/// To make an OpenCAN device plugin, create a subclass of QObject
/// and OcDeviceInterface. Then, create a subclass of OcDevice and
/// implement the hardware-specific methods for controlling the
/// device.
class OPENCAN_EXPORT OcDeviceInterface
{
public:
    virtual ~OcDeviceInterface() = default;

	/// Virtual method for retrieving a plugin's device type string
	virtual QString type() const = 0;

	/// Get the OcPluginInfo for this plugin
    /// The returned OcPluginInfo should not be deleted, it will
    /// be automatically deleted when the plugin is unloaded.
    virtual OcPluginInfo* info() = 0;

	/// Discover devices
	virtual void discoverDevices() = 0;

	/// Get the list of discovered devices
    /// The OcDevices in the returned list should not be deleted,
    /// they will be automatically deleted when the plugin is unloaded.
	virtual OcDeviceList getDevices() = 0;

	/// Virtual method for retrieving a plugin's CAN device
	/// @param device the device name
    /// The returned OcDevice should not be deleted, it will
    /// be automatically deleted when the plugin is unloaded.
	virtual OcDevice* getDevice(const QString &device) const = 0;
};

Q_DECLARE_INTERFACE(OcDeviceInterface, OcDeviceInterface_iid)

OPENCAN_HEADER_END

#endif // OCDEVICEINTERFACE_H

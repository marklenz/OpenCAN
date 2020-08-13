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

#ifndef OCDEVICEMANAGER_H
#define OCDEVICEMANAGER_H

#include <QList>
#include <QHash>
#include <QDir>

#include <ocdeviceinterface.h>

OPENCAN_HEADER_BEGIN

typedef QList<OcPluginInfo *> OcPluginInfoList;

/// Manages all OpenCAN CAN devices and plugins.

/// The OcDeviceManager is the "entry point" of the OpenCAN API.
/// Create an instance of OcDeviceManager to first load all available
/// OpenCAN device plugins and then access the desired device(s).
class OPENCAN_EXPORT OcDeviceManager
{
public:
	OcDeviceManager();
	virtual ~OcDeviceManager();

	static QString version();

	void loadPlugins();
	OcPluginInfoList getPluginInfos() const;
	OcDeviceList getDevices() const;

	OcDevice* getDevice(const QString &type, const QString &device);

private:
    void loadPlugins(const QDir &dir);
    void loadDeviceInterface(OcDeviceInterface *iface, const QString &fileName = QString());

	QHash<QString, OcDeviceInterface *> plugins;
	OcPluginInfoList pluginInfos;
	OcDeviceList devices;
};

// Inline OcDeviceManager Methods

/// Get the OcPluginInfo for each plugin file installed
inline OcPluginInfoList OcDeviceManager::getPluginInfos() const { return pluginInfos; }

/// Get the OcDeviceInfo for each plugin file installed
inline OcDeviceList OcDeviceManager::getDevices() const { return devices; }

OPENCAN_HEADER_END

#endif // OCDEVICEMANAGER_H

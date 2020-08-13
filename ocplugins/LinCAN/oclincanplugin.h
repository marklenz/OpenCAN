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

#ifndef OCLINCANPLUGIN_H
#define OCLINCANPLUGIN_H

#include <QtPlugin>

#include <ocdeviceinterface.h>

/// LinCAN device plugin class
class OcLinCanPlugin : public QObject, public OcDeviceInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID OcDeviceInterface_iid FILE "LinCAN.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(OcDeviceInterface)
public:
    explicit OcLinCanPlugin(QObject *parent = 0);
	~OcLinCanPlugin();

	/// Get the CAN device type string - "lincan"
    QString type() const override { return pluginInfo.type(); }

    /// Get a pointer to the OcPluginInfo for this plugin
    OcPluginInfo* info() override { return &pluginInfo; }

	/// Discover devices
    void discoverDevices() override;

	/// Get the list of discovered devices
    OcDeviceList getDevices() override { return devices; }

    OcDevice* getDevice(const QString &device) const override;

private:
    OcPluginInfo pluginInfo;
	OcDeviceList devices;
};

#endif // OCLINCANPLUGIN_H

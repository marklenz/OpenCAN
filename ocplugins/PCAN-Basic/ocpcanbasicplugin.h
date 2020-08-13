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

#ifndef OCPCANBASICPLUGIN_H
#define OCPCANBASICPLUGIN_H

#include <QtPlugin>

#include <ocdeviceinterface.h>

/// PCAN-Basic device plugin class
class OcPcanBasicPlugin : public QObject, public OcDeviceInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID OcDeviceInterface_iid FILE "PCAN-Basic.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(OcDeviceInterface)
public:
    explicit OcPcanBasicPlugin(QObject *parent = 0);
    ~OcPcanBasicPlugin();

    /// Get the CAN device type string - "pcan"
    QString type() const override { return pluginInfo.type(); }

    /// Get the OcPluginInfo for this plugin
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

#endif // OCPCANBASICPLUGIN_H

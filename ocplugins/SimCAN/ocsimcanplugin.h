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

#ifndef OCSIMCANPLUGIN_H
#define OCSIMCANPLUGIN_H

#include <QtPlugin>

#include <ocdeviceinterface.h>

/// SimCAN device plugin class
class OcSimCANPlugin : public QObject, public OcDeviceInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID OcDeviceInterface_iid FILE "SimCAN.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(OcDeviceInterface)
public:
    explicit OcSimCANPlugin(QObject *parent = 0);
    ~OcSimCANPlugin();

    /// Get the CAN device type string - "sim"
    QString type() const override { return pluginInfo.type(); }

    /// Get a pointer to the OcPluginInfo for this plugin
    OcPluginInfo* info() override { return &pluginInfo; }

    /// Discover devices
    void discoverDevices() override;

    /// Get the list of discovered devices
    OcDeviceList getDevices() override { return devices; }

    OcDevice* getDevice(const QString &num) const override;

private:
    OcPluginInfo pluginInfo;
    OcDeviceList devices;
};

#endif // OCSIMCANPLUGIN_H

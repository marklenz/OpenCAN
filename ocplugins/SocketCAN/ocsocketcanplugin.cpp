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

#include "ocsocketcanplugin.h"

#include "ocsocketcandevice.h"

/// The CAN device string
static const QString DEV_SIMCAN = QStringLiteral("socket");

/// The OpenCAN plugin name
static const QString PLUGIN_TEXT = QStringLiteral("OpenCAN SocketCAN Device Plugin");

OcSocketCanPlugin::OcSocketCanPlugin(QObject *parent) :
    QObject(parent),
    pluginInfo(PLUGIN_TEXT, DEV_SIMCAN, OpenCAN_Version(), Result::Success)
{

}

OcSocketCanPlugin::~OcSocketCanPlugin()
{
    foreach (OcDevice *device, devices)
        delete device;
}

void OcSocketCanPlugin::discoverDevices()
{
    // don't add a new device if one already exists
    while (devices.size() < NUM_SOCKETCAN_DEVICES)
        devices.append(new OcSocketCanDevice(QString::number(devices.size() + 1)));
}

/// Get an OpenCAN SocketCAN device
/// @param device the device number
OcDevice* OcSocketCanPlugin::getDevice(const QString &num) const
{
    int index = num.toInt() - 1;
    if (index < devices.size())
        return devices.at(index);

    return nullptr;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(IPC, OcSocketCANPlugin);
#endif // QT_VERSION < 0x050000

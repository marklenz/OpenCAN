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

#include "ocpcanlightplugin.h"

#include <qplugin.h>

#include "ocpcanlightdevice.h"

/// The CAN device string
static const QString DEV_PCAN = QStringLiteral("pcan-light");

/// The OpenCAN plugin name
static const QString PLUGIN_TEXT = QStringLiteral("OpenCAN PCAN-Light Device Plugin");

static const QString USB_DEVICE_ID = QStringLiteral("32");

OcPcanLightPlugin::OcPcanLightPlugin(QObject *parent) :
    QObject(parent),
    pluginInfo(PLUGIN_TEXT, DEV_PCAN, OpenCAN_Version(), Result::Success)
{

}

OcPcanLightPlugin::~OcPcanLightPlugin()
{
    foreach (OcDevice *device, devices)
		delete device;
}

void OcPcanLightPlugin::discoverDevices()
{
    // don't add a new device if one already exists
    if (devices.size() == 0)
        devices.append(new OcPcanLightDevice(USB_DEVICE_ID));
}

/// Get an OpenCAN PCAN-Light device
/// The QString parameter is ignored
OcDevice* OcPcanLightPlugin::getDevice(const QString &) const
{
	return devices.at(0);
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(PCAN, OcPCanLightPlugin);
#endif // QT_VERSION < 0x050000


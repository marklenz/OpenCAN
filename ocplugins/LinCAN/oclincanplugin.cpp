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

#include "oclincanplugin.h"

#include <qplugin.h>

#include "oclincandevice.h"

using namespace OpenCAN;

/// The CAN device string
static const QString DEV_LINCAN = QStringLiteral("lincan");

/// The OpenCAN plugin name
static const QString PLUGIN_TEXT = QStringLiteral("OpenCAN LinCAN Device Plugin");

static const QString DEVICE_0 = QStringLiteral("0");
static const QString DEVICE_1 = QStringLiteral("1");

OcLinCanPlugin::OcLinCanPlugin(QObject *parent) :
    QObject(parent),
    pluginInfo(PLUGIN_TEXT, DEV_LINCAN, OpenCAN_Version(), Result::Success)
{

}

OcLinCanPlugin::~OcLinCanPlugin()
{
    foreach (OcDevice *device, devices)
		delete device;
}

void OcLinCanPlugin::discoverDevices()
{
    // don't add a new device if one already exists
    if (devices.size() == 0)
    {
        // TODO: check if file exists
        devices.append(new OcLinCanDevice(DEVICE_0));

        // TODO: check if file exists
        devices.append(new OcLinCanDevice(DEVICE_1));
    }
}

/// Get an OpenCAN LinCAN device
/// @param device the device name
OcDevice* OcLinCanPlugin::getDevice(const QString &device) const
{
    if (device == DEVICE_0)
		return devices.at(0);
    else if (device == DEVICE_1)
		return devices.at(1);

	return nullptr;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(LinCAN, OcLinCanPlugin);
#endif // QT_VERSION < 0x050000


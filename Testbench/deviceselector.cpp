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
**  Licensees holding valid commercial OpenCAN and Testbench licenses may use
**  this file in accordance with the commercial license agreement or other
**  written agreement between you and Code of Intelligence that governs your
**  use of the Software.  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#include "deviceselector.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

DeviceSelector::DeviceSelector(const OcDeviceList &devices, QWidget *parent) :
    QWidget(parent)
{
	connectedDevs = devices;

    auto layout = new QHBoxLayout;
	layout->addWidget(new QLabel(tr("Device")));
	devBox = new QComboBox;
	foreach (OcDevice *device, connectedDevs)
		devBox->addItem(device->text());
    connect(devBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DeviceSelector::indexChanged);
	layout->addWidget(devBox);
	layout->addStretch(10);

	setLayout(layout);
}

void DeviceSelector::deviceConnected(OcDevice *device)
{
	connectedDevs.append(device);
	devBox->addItem(device->text());
}

void DeviceSelector::deviceDisconnected(OcDevice *device)
{
	int index = connectedDevs.indexOf(device);
	if (index > -1)
	{
        device->stop();
		connectedDevs.removeAll(device);
		devBox->removeItem(index);
	}
}

void DeviceSelector::indexChanged(int index)
{
	if (index > -1)
		emit deviceChanged(connectedDevs.at(index));
	else
		emit deviceChanged(nullptr);
}

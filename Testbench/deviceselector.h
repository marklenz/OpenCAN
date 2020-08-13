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

#ifndef DEVICESELECTOR_H
#define DEVICESELECTOR_H

#include <QWidget>

#include <ocdevice.h>

class QComboBox;

class DeviceSelector : public QWidget
{
    Q_OBJECT
public:
	explicit DeviceSelector(const OcDeviceList &devices, QWidget *parent = 0);

	void deviceConnected(OcDevice *device);
	void deviceDisconnected(OcDevice *device);

signals:
	void deviceChanged(OcDevice *);

private slots:
	void indexChanged(int index);

private:
	OcDeviceList connectedDevs;
	QComboBox *devBox;
};

#endif // DEVICESELECTOR_H

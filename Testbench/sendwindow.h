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

#ifndef SENDWINDOW_H
#define SENDWINDOW_H

#include <QWidget>

#include "deviceselector.h"

class QPushButton;

class BaseSpinBox;

class SendWindow : public QWidget
{
Q_OBJECT
public:
	explicit SendWindow(const OcDeviceList &devices, QWidget *parent = 0);

public slots:
	void deviceConnected(OcDevice *device);
	void deviceDisconnected(OcDevice *device);

private slots:
	void deviceChanged(OcDevice *device);
	void sendMessage();

private:
	OcDeviceList connectedDevs;
	OcDevice *selectedDev;

	DeviceSelector *devSelect;
	BaseSpinBox *msgID, *byte0, *byte1, *byte2, *byte3, *byte4, *byte5, *byte6, *byte7;
	QPushButton *sendBtn;
};

#endif // SENDWINDOW_H

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

#ifndef MANAGEMENTWINDOW_H
#define MANAGEMENTWINDOW_H

#include <QWidget>

class QPushButton;
class QComboBox;
class QCheckBox;

class OcDeviceManager;
class OcDevice;

class ManagementWindow : public QWidget
{
Q_OBJECT
public:
	explicit ManagementWindow(OcDeviceManager *manager, QWidget *parent = 0);

signals:
	void changeCurrentDevice(int);
	void connected(OcDevice *);
	void disconnected(OcDevice *);
	void configChanged(OcDevice *);

private slots:
	void connectDevice();
	void deviceChanged(int index);
	void baudChanged(int index);
	void extChanged(bool extended);
    void loopbackChanged(bool loopback);

private:
	void loadDeviceDetails();
	void updateConnectStatus(OcDevice *device);

	OcDeviceManager *deviceManager;

	int devIndex;
	bool inUpdate;
    QList<int> bauds;

	QPushButton *connectBtn;
	QComboBox *baudBox;
    QCheckBox *extCANBtn, *loopbackBtn;
};

#endif // MANAGEMENTWINDOW_H

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

#include "sendwindow.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include <ocmessage.h>

#include "basespinbox.h"

static const quint32 MAX_EXT = 0x1FFFFFFF;
static const quint32 MAX_STD = 0x7FF;

static const QString PREFIX_TX = QStringLiteral("Tx: ");

SendWindow::SendWindow(const OcDeviceList &devices, QWidget *parent) :
    QWidget(parent)
{
	connectedDevs = devices;
    selectedDev = nullptr;

    auto layout = new QVBoxLayout;
	devSelect = new DeviceSelector(connectedDevs);
    connect(devSelect, &DeviceSelector::deviceChanged, this, &SendWindow::deviceChanged);
	layout->addWidget(devSelect);

    auto msgLt = new QHBoxLayout;

    auto numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("ID:")));
	msgID = new BaseSpinBox(16);
	msgID->setRange(0, MAX_EXT);
	numLt->addWidget(msgID);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 1:")));
	byte0 = new BaseSpinBox(16);
	byte0->setRange(0, 0xFF);
	numLt->addWidget(byte0);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 2:")));
	byte1 = new BaseSpinBox(16);
	byte1->setRange(0, 0xFF);
	numLt->addWidget(byte1);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 3:")));
	byte2 = new BaseSpinBox(16);
	byte2->setRange(0, 0xFF);
	numLt->addWidget(byte2);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 4:")));
	byte3 = new BaseSpinBox(16);
	byte3->setRange(0, 0xFF);
	numLt->addWidget(byte3);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 5:")));
	byte4 = new BaseSpinBox(16);
	byte4->setRange(0, 0xFF);
	numLt->addWidget(byte4);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 6:")));
	byte5 = new BaseSpinBox(16);
	byte5->setRange(0, 0xFF);
	numLt->addWidget(byte5);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 7:")));
	byte6 = new BaseSpinBox(16);
	byte6->setRange(0, 0xFF);
	numLt->addWidget(byte6);
	msgLt->addLayout(numLt);

	numLt = new QVBoxLayout;
	numLt->addWidget(new QLabel(tr("Byte 8:")));
	byte7 = new BaseSpinBox(16);
	byte7->setRange(0, 0xFF);
	numLt->addWidget(byte7);
	msgLt->addLayout(numLt);
	sendBtn = new QPushButton(tr("Send"));
	msgLt->addWidget(sendBtn);
    //connect(sendBtn, &QPushButton::clicked, this, &SendWindow::sendMessage);
    connect(sendBtn, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));
    layout->addLayout(msgLt);

	setLayout(layout);

	if (!connectedDevs.isEmpty())
		deviceChanged(connectedDevs.at(0));
	else
        deviceChanged(nullptr);
}

void SendWindow::deviceConnected(OcDevice *device)
{
	devSelect->deviceConnected(device);
	connectedDevs.append(device);

	sendBtn->setEnabled(true);
}

void SendWindow::deviceDisconnected(OcDevice *device)
{
	devSelect->deviceDisconnected(device);
	connectedDevs.removeAll(device);

	if (connectedDevs.isEmpty())
		sendBtn->setEnabled(false);
}

void SendWindow::deviceChanged(OcDevice *device)
{
	selectedDev = device;
	if (selectedDev)
	{
        setWindowTitle(PREFIX_TX + selectedDev->text());

		if (selectedDev->isExtended())
			msgID->setRange(0, MAX_EXT);
		else
			msgID->setRange(0, MAX_STD);
	}
	else
        setWindowTitle(PREFIX_TX + tr("No Device"));
}

void SendWindow::sendMessage()
{
    if (selectedDev)
	{
		int id = msgID->value();
		int b0 = byte0->value();
		int b1 = byte1->value();
		int b2 = byte2->value();
		int b3 = byte3->value();
		int b4 = byte4->value();
		int b5 = byte5->value();
		int b6 = byte6->value();
		int b7 = byte7->value();
		OcMessage msg(id, b0, b1, b2, b3, b4, b5, b6, b7);
		msg.setExtended(selectedDev->isExtended());
		selectedDev->sendMessage(msg);
	}
}

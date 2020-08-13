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

#include "managementwindow.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include <ocdevicemanager.h>

using namespace OpenCAN;

static const QString SIMCAN_DEVICE_TEXT = QStringLiteral("SimCAN");

ManagementWindow::ManagementWindow(OcDeviceManager *manager, QWidget *parent) :
    QWidget(parent)
{
	devIndex = 0;
	inUpdate = false;

	deviceManager = manager;

    auto layout = new QGridLayout;

    layout->addWidget(new QLabel(QStringLiteral("OpenCAN Version")), 0, 0);
	layout->addWidget(new QLabel(OpenCAN_Version()), 0, 1);

	layout->addWidget(new QLabel(tr("Device")), 1, 0);
	OcDeviceList devices = deviceManager->getDevices();
    auto devBox = new QComboBox();
	foreach (OcDevice *device, devices)
	{
        device->setAutonomous(true);
		devBox->addItem(device->text());
	}
    connect(devBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ManagementWindow::deviceChanged);
	layout->addWidget(devBox, 1, 1);

	connectBtn = new QPushButton(tr("Connect"));
    connect(connectBtn, &QPushButton::clicked, this, &ManagementWindow::connectDevice);
	layout->addWidget(connectBtn, 1, 2);

	layout->addWidget(new QLabel(tr("Baud Rate")), 2, 0);
	baudBox = new QComboBox();
    connect(baudBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ManagementWindow::baudChanged);
	layout->addWidget(baudBox, 2, 1);

	layout->addWidget(new QLabel(tr("Extended Frame")), 3, 0);
	extCANBtn = new QCheckBox();
    connect(extCANBtn, &QPushButton::toggled, this, &ManagementWindow::extChanged);
	layout->addWidget(extCANBtn, 3, 1);

    layout->addWidget(new QLabel(tr("Loopback")), 4, 0);
    loopbackBtn = new QCheckBox();
    connect(loopbackBtn, &QPushButton::toggled, this, &ManagementWindow::loopbackChanged);
    layout->addWidget(loopbackBtn, 4, 1);

	setLayout(layout);

	loadDeviceDetails();
}

void ManagementWindow::connectDevice()
{
    if (deviceManager->getDevices().isEmpty())
		return;

	OcDevice *device = deviceManager->getDevices().at(devIndex);

	if (!device)
	{
		connectBtn->setDown(false);
		return;
	}

    if (!bauds.isEmpty())
    {
        // set baud rate
        if (bauds.at(baudBox->currentIndex()) == -1)
        {
            // arbitrary baud rate
            baudBox->setEditable(true);

            bool ok;
            int rate = baudBox->itemText(baudBox->currentIndex()).toInt(&ok);
            if (ok && rate > 0 && rate <= (int)BaudRate::Baud_1M)
                device->setBaudRate(rate);
            else
                baudBox->setItemText(baudBox->currentIndex(), QString::number(device->getBaudRate()));
        }
        else
        {
            baudBox->setEditable(false);

            device->setBaudRate(bauds.at(baudBox->currentIndex()));
        }
    }

    OpenCAN::Result result;
	if (!device->isStarted())
    {
        result = device->start();
    }
    else
        result = device->stop();

    if (result == OpenCAN::Result::Success)
        updateConnectStatus(device);
}

void ManagementWindow::deviceChanged(int index)
{
	devIndex = index;

	loadDeviceDetails();

	emit changeCurrentDevice(devIndex);
}

void ManagementWindow::baudChanged(int index)
{
    if (inUpdate)
    	return;

    if (!bauds.isEmpty())
        baudBox->setEditable(bauds.at(index) == -1);
}

void ManagementWindow::extChanged(bool extended)
{
	if (inUpdate)
		return;

	OcDevice *device = deviceManager->getDevices().at(devIndex);
	device->setExtended(extended);

	emit configChanged(device);
}

void ManagementWindow::loopbackChanged(bool loopback)
{
    if (inUpdate)
        return;

    OcDevice *device = deviceManager->getDevices().at(devIndex);
    device->setLoopback(loopback);
}

void ManagementWindow::loadDeviceDetails()
{
	if (deviceManager->getDevices().isEmpty())
		return;

	inUpdate = true;

	OcDevice *device = deviceManager->getDevices().at(devIndex);
    updateConnectStatus(device);

    bauds = device->baudRates();
    baudBox->clear();
    if (bauds.isEmpty() && !device->text().startsWith(SIMCAN_DEVICE_TEXT))
    {
        bauds.append((int)BaudRate::Baud_1M);	//   1 MBit/s
        bauds.append((int)BaudRate::Baud_500K);	// 500 kBit/s
        bauds.append((int)BaudRate::Baud_250K);	// 250 kBit/s
        bauds.append((int)BaudRate::Baud_125K);	// 125 kBit/s
        bauds.append((int)BaudRate::Baud_100K);	// 100 kBit/s
        bauds.append((int)BaudRate::Baud_50K);	//  50 kBit/s
        bauds.append((int)BaudRate::Baud_20K);	//  20 kBit/s
        bauds.append((int)BaudRate::Baud_10K);	//  10 kBit/s
        bauds.append((int)BaudRate::Baud_5K);	//   5 kBit/s
        bauds.append(-1);           // Arbitrary
    }
	foreach (int baud, bauds)
    {
        if (baud == (int)BaudRate::Baud_1M)
            baudBox->addItem(QStringLiteral("1 Mb/s"));
        else if (baud == -1)
            baudBox->addItem(tr("Other"));
        else
            baudBox->addItem(QStringLiteral("%1 kb/s").arg(baud / 1000));
    }
    baudBox->setCurrentIndex(bauds.indexOf(device->getBaudRate()));

	extCANBtn->setChecked(device->isExtended());
    loopbackBtn->setChecked(device->isLoopback());

    inUpdate = false;
}

void ManagementWindow::updateConnectStatus(OcDevice *device)
{
	if (device->isStarted())
	{
		connectBtn->setText(tr("Disconnect"));
		connectBtn->setDown(true);

		baudBox->setEnabled(false);
		extCANBtn->setEnabled(false);

		emit connected(device);
	}
	else
	{
		connectBtn->setText(tr("Connect"));
		connectBtn->setDown(false);

		baudBox->setEnabled(true);
		extCANBtn->setEnabled(true);

		emit disconnected(device);
	}
}

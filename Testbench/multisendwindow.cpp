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

#include "multisendwindow.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "sendmessagewidget.h"

static const QString PREFIX_TX = QStringLiteral("Tx: ");

MultiSendWindow::MultiSendWindow(const OcDeviceList &devices, QWidget *parent) : QWidget(parent)
{
    connectedDevs = devices;
    selectedDev = nullptr;

    layout = new QVBoxLayout;

    auto topLayout = new QHBoxLayout;
    devSelect = new DeviceSelector(connectedDevs);
    connect(devSelect, &DeviceSelector::deviceChanged, this, &MultiSendWindow::deviceChanged);
    topLayout->addWidget(devSelect);
    auto addBtn = new QPushButton(QStringLiteral("+"));
    connect(addBtn, &QPushButton::clicked, this, &MultiSendWindow::addMessage);
    topLayout->addWidget(addBtn);
    layout->addLayout(topLayout);

    auto msgLt = new QHBoxLayout;
    msgLt->addWidget(new QLabel(tr("ID")));
    //msgLt->insertSpacing(0, 1);
    msgLt->insertSpacing(1, 50);
    msgLt->addWidget(new QLabel(tr("Byte 1")));
    msgLt->addWidget(new QLabel(tr("Byte 2")));
    msgLt->addWidget(new QLabel(tr("Byte 3")));
    msgLt->addWidget(new QLabel(tr("Byte 4")));
    msgLt->addWidget(new QLabel(tr("Byte 5")));
    msgLt->addWidget(new QLabel(tr("Byte 6")));
    msgLt->addWidget(new QLabel(tr("Byte 7")));
    msgLt->addWidget(new QLabel(tr("Byte 8")));
    msgLt->addWidget(new QLabel(tr("Timer")));
    msgLt->addWidget(new QLabel(tr("Send")));
    layout->addLayout(msgLt);

    setLayout(layout);

    if (!connectedDevs.isEmpty())
        deviceChanged(connectedDevs.at(0));
    else
        deviceChanged(nullptr);
}

void MultiSendWindow::deviceConnected(OcDevice *device)
{
    devSelect->deviceConnected(device);
    connectedDevs.append(device);
}

void MultiSendWindow::deviceDisconnected(OcDevice *device)
{
    devSelect->deviceDisconnected(device);
    connectedDevs.removeAll(device);
}

void MultiSendWindow::deviceChanged(OcDevice *device)
{
    selectedDev = device;
    if (selectedDev)
        setWindowTitle(PREFIX_TX + selectedDev->text());
    else
        setWindowTitle(PREFIX_TX + tr("No Device"));
}

void MultiSendWindow::addMessage()
{
    SendMessageWidget *msg = new SendMessageWidget();
    connect(msg, &SendMessageWidget::sendMessage, this, &MultiSendWindow::sendMessage);
    layout->addWidget(msg);
}

void MultiSendWindow::sendMessage(OcMessage &message)
{
    if (selectedDev)
    {
        message.setExtended(selectedDev->isExtended());
        selectedDev->sendMessage(message);
    }
}

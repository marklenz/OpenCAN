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

#include "sendmessagewidget.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "basespinbox.h"
#include "ocmessage.h"

static const quint32 MAX_EXT = 0x1FFFFFFF;
static const quint32 MAX_STD = 0x7FF;

SendMessageWidget::SendMessageWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    msgId = new BaseSpinBox(16);
    msgId->setRange(0, MAX_EXT);
    layout->addWidget(msgId);

    byte1 = new BaseSpinBox(16);
    byte1->setRange(0, 0xFF);
    layout->addWidget(byte1);

    byte2 = new BaseSpinBox(16);
    byte2->setRange(0, 0xFF);
    layout->addWidget(byte2);

    byte3 = new BaseSpinBox(16);
    byte3->setRange(0, 0xFF);
    layout->addWidget(byte3);

    byte4 = new BaseSpinBox(16);
    byte4->setRange(0, 0xFF);
    layout->addWidget(byte4);

    byte5 = new BaseSpinBox(16);
    byte5->setRange(0, 0xFF);
    layout->addWidget(byte5);

    byte6 = new BaseSpinBox(16);
    byte6->setRange(0, 0xFF);
    layout->addWidget(byte6);

    byte7 = new BaseSpinBox(16);
    byte7->setRange(0, 0xFF);
    layout->addWidget(byte7);

    byte8 = new BaseSpinBox(16);
    byte8->setRange(0, 0xFF);
    layout->addWidget(byte8);

    sendTime = new QSpinBox;
    sendTime->setRange(0, 1000000);
    //layout->insertSpacing(11, 20);
    connect(sendTime, SIGNAL(valueChanged(int)), this, SLOT(timerChanged(int)));
    layout->addWidget(sendTime);

    enableSend = new QCheckBox;
    enableSend->setChecked(true);
    enableSend->hide();
    connect(enableSend, &QCheckBox::stateChanged, this, &SendMessageWidget::enableSendChanged);
    layout->addWidget(enableSend);

    sendMsg = new QPushButton(tr("Send"));
    connect(sendMsg, &QPushButton::clicked, this, &SendMessageWidget::sendMessageSlot);
    layout->addWidget(sendMsg);

    setLayout(layout);

    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &SendMessageWidget::sendMessageSlot);
}

void SendMessageWidget::sendMessageSlot()
{
    int id = msgId->value();
    int b1 = byte1->value();
    int b2 = byte2->value();
    int b3 = byte3->value();
    int b4 = byte4->value();
    int b5 = byte5->value();
    int b6 = byte6->value();
    int b7 = byte7->value();
    int b8 = byte8->value();
    OcMessage msg(id, b1, b2, b3, b4, b5, b6, b7, b8);
    sendMessage(msg);
}

void SendMessageWidget::timerChanged(int t)
{
    if (sendTime->value() == 0)
    {
        enableSend->hide();
        sendMsg->show();

        sendTimer->stop();
    }
    else
    {
        enableSend->show();
        sendMsg->hide();

        if (enableSend->isChecked())
            startTimer();
    }
}

void SendMessageWidget::startTimer()
{
    sendTimer->setInterval(sendTime->value());
    sendTimer->start();
}

void SendMessageWidget::enableSendChanged(int state)
{
    if (Qt::Unchecked == state)
        sendTimer->stop();
    else
        startTimer();
}

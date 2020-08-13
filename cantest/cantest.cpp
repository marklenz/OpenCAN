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

#include "cantest.h"

#include <QTimer>

#include "ocdevice.h"

static const int SEND_PING_MSECS = 1000;

CanTest::CanTest(QObject *parent) : QObject(parent)
{
    deviceMgr.loadPlugins();

    device = deviceMgr.getDevices().at(0);
    device->setAutonomous(true);
    connect(device, &OcDevice::messageReceived, this, &CanTest::processMessage);
    device->start();

    pingTimer = new QTimer(this);
    pingTimer->setInterval(SEND_PING_MSECS);
    connect(pingTimer, &QTimer::timeout, this, &CanTest::sendPing);
    pingTimer->start();
}

void CanTest::processMessage(const OcMessage &msg)
{
    OcMessage response(msg.id() + 1);
    device->sendMessage(response);
}

void CanTest::sendPing()
{
    OcMessage msg(0xFF, 0, 0, 0, 0, 0, 0, 0, 0);
    device->sendMessage(msg);
}

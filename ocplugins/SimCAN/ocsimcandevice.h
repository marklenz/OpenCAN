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
**  Licensees holding valid commercial OpenCAN and SimCAN licenses may use
**  this file in accordance with the commercial license agreement or other
**  written agreement between you and Code of Intelligence that governs your
**  use of the Software.  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#ifndef OCSIMCANDEVICE_H
#define OCSIMCANDEVICE_H

#include <ocdevice.h>

#include "ocsimcanmessagebuffer.h"

static const int NUM_SIMCAN_DEVICES = 3;

using namespace OpenCAN;

/// SimCAN device class
class OcSimCANDevice : public OcDevice
{
    Q_OBJECT
public:
    explicit OcSimCANDevice(const QString &deviceNum);
    virtual ~OcSimCANDevice();

    Result setBaudRate(qint32) override { return Result::Success; } // no baud rate
    Result getMessage(OcMessage &msg) override;

private:
    Result initialize() override;
    Result uninitialize() override;
    Result internalSendMessage(const OcMessage &msg) override;

    OcSimCANMessageBuffer buffer;
};

#endif // OCSIMCANDEVICE_H

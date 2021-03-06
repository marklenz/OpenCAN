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

#ifndef OCSOCKETCANDEVICE_H
#define OCSOCKETCANDEVICE_H

#include <ocdevice.h>

static const int NUM_SOCKETCAN_DEVICES = 1;

using namespace OpenCAN;

class OcSocketCanDevice : public OcDevice
{
    Q_OBJECT
public:
    explicit OcSocketCanDevice(const QString &deviceId);
    virtual ~OcSocketCanDevice();

    Result setBaudRate(qint32 baud) override;
    Result getMessage(OcMessage &msg) override;

private:
    Result initialize() override;
    Result uninitialize() override;
    Result internalSendMessage(const OcMessage &msg) override;

    int sock;
    QString devFile;
};

#endif // OCSOCKETCANDEVICE_H

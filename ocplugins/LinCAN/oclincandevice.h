/******************************************************************************
**
**  Copyright (C) 2005-2020 Code of Intelligence, LLC.
**  Contact: contact@opencan.org
**
**  This file may be used under the terms of the GNU Lesser General Public
**  License version 2.1 as published by the Free Software Foundation and
**  appearing in the file LICENSE included in the packaging of this file.
**  Please review the following information to ensure the GNU Lesser General
**  Public License version 2.1 requirements will be met:
**  http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
******************************************************************************/

#ifndef OCLINCANDEVICE_H
#define OCLINCANDEVICE_H

#include <ocdevice.h>

using namespace OpenCAN;

/// LinCAN device class
class OcLinCanDevice : public OcDevice
{
    Q_OBJECT
public:
    explicit OcLinCanDevice(const QString &deviceId);
    virtual ~OcLinCanDevice();

    Result setBaudRate(qint32 baud) override;
    Result getMessage(OcMessage &msg) override;

private:
    Result initialize() override;
    Result uninitialize() override;
    Result internalSendMessage(const OcMessage &msg) override;

    int fd;
	QString devFile;
};

#endif // OCLINCANDEVICE_H

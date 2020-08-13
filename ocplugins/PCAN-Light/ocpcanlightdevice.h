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

#ifndef OCPCANLIGHTDEVICE_H
#define OCPCANLIGHTDEVICE_H

#include <ocdevice.h>

using namespace OpenCAN;

/// PCAN-Light device class
class OcPcanLightDevice : public OcDevice
{
    Q_OBJECT
public:
    explicit OcPcanLightDevice(const QString &deviceId);
    virtual ~OcPcanLightDevice();

    Result setBaudRate(qint32 baud) override;
    Result getMessage(OcMessage &msg) override;

private:
    Result initialize() override;
    Result uninitialize() override;
    Result internalSendMessage(const OcMessage &msg) override;

    Result setBaudRateLinux();
    void setTimestamp(quint32 msecs);

    qint32 baudRate;
    quint32 prevTimestamp;
    QTime prevTime;
#if defined(Q_OS_UNIX)
    void *fd;
    QString devFile;
#endif
};

#endif // OCPCANLIGHTDEVICE_H

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

#ifndef MULTISENDWINDOW_H
#define MULTISENDWINDOW_H

#include <QWidget>

#include "deviceselector.h"

class QVBoxLayout;

class MultiSendWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MultiSendWindow(const OcDeviceList &devices, QWidget *parent = nullptr);

public slots:
    void deviceConnected(OcDevice *device);
    void deviceDisconnected(OcDevice *device);

private slots:
    void deviceChanged(OcDevice *device);
    void addMessage();
    void sendMessage(OcMessage &message);

private:
    OcDeviceList connectedDevs;
    OcDevice *selectedDev;

    DeviceSelector *devSelect;

    QVBoxLayout *layout;
};

#endif // MULTISENDWINDOW_H

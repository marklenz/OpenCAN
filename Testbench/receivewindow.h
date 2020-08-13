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

#ifndef RECEIVEWINDOW_H
#define RECEIVEWINDOW_H

#include <QWidget>

#include "deviceselector.h"

class QCheckBox;
class QRadioButton;
class QPlainTextEdit;
class QPushButton;

class BaseSpinBox;

class ReceiveWindow : public QWidget
{
Q_OBJECT
public:
	explicit ReceiveWindow(const OcDeviceList &devices, QWidget *parent = 0);

public slots:
	void deviceConnected(OcDevice *device);
	void deviceDisconnected(OcDevice *device);

private slots:
	void clickedAllMsgs();
	void clickedAllData();

	void deviceChanged(OcDevice *device);
    void processMessage(const OcMessage &msg);

	void start();

private:
    QFont getMonospaceFont();
    void startListening();
    void stopListening();
    bool passFilter(const OcMessage &msg);
    bool passDataFilter(const OcMessage &msg);

	OcDeviceList connectedDevs;
    OcDevice *selectedDevice;
    bool started;

    DeviceSelector *selectDevBox;
	QPushButton *startBtn;
	QCheckBox *allMsgs, *allData;
	QWidget *idBox, *dataBox;
	BaseSpinBox *idEdit, *idMaskEdit,
				*b1Edit, *m1Edit, *b2Edit, *m2Edit, *b3Edit, *m3Edit, *b4Edit, *m4Edit,
				*b5Edit, *m5Edit, *b6Edit, *m6Edit, *b7Edit, *m7Edit, *b8Edit, *m8Edit;
    QPlainTextEdit *box;
};

#endif // RECEIVEWINDOW_H

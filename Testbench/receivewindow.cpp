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

#include "receivewindow.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "basespinbox.h"

static const int VIEW_BASE = 16;

static const int MAX_MESSAGES_DISPLAYED = 1000;

static const quint32 MAX_EXT = 0x1FFFFFFF;
static const quint32 MAX_STD = 0x7FF;

static const QString PREFIX_RX = QStringLiteral("Rx: ");

ReceiveWindow::ReceiveWindow(const OcDeviceList &devices, QWidget *parent) :
    QWidget(parent)
{
	connectedDevs = devices;
    selectedDevice = nullptr;
    started = false;

    auto layout = new QVBoxLayout;

    auto devLt = new QHBoxLayout;
    selectDevBox = new DeviceSelector(connectedDevs);
    connect(selectDevBox, &DeviceSelector::deviceChanged, this, &ReceiveWindow::deviceChanged);
    devLt->addWidget(selectDevBox);
	startBtn = new QPushButton(tr("Start"));
    connect(startBtn, &QPushButton::clicked, this, &ReceiveWindow::start);
	devLt->addWidget(startBtn);
	devLt->addStretch(10);
	layout->addLayout(devLt);

    allMsgs = new QCheckBox(tr("All Identifiers"));
	allMsgs->setChecked(true);
	layout->addWidget(allMsgs);
    connect(allMsgs, &QCheckBox::clicked, this, &ReceiveWindow::clickedAllMsgs);

	//idBox = new QGroupBox(tr("Identifier"));
	idBox = new QWidget;
    auto msgLt = new QVBoxLayout;

    auto idLt = new QHBoxLayout;
	idLt->addWidget(new QLabel(tr("Message ID")));
	idEdit = new BaseSpinBox(16);
	idEdit->setMaximum(MAX_EXT);
	idEdit->setValue(MAX_EXT);
    //connect(idEdit, SIGNAL(valueChanged(const QString&)), this, SLOT(valueChanged(const QString&)));
	idLt->addWidget(idEdit);
	idLt->addStretch(1);
	idLt->addWidget(new QLabel(tr("Mask")));
	idMaskEdit = new BaseSpinBox(16);
	idMaskEdit->setMaximum(MAX_EXT);
	idMaskEdit->setValue(MAX_EXT);
    //connect(idMaskEdit, SIGNAL(valueChanged(const QString&)), this, SLOT(valueChanged(const QString&)));
	idLt->addWidget(idMaskEdit);
	idLt->addStretch(3);
	msgLt->addLayout(idLt);

	idBox->setLayout(msgLt);
	idBox->hide();
	layout->addWidget(idBox);

	allData = new QCheckBox(tr("All Data"));
	allData->setChecked(true);
	layout->addWidget(allData);

	//dataBox = new QGroupBox(tr("Data Filter"));
	dataBox = new QWidget;
    auto dataLt = new QVBoxLayout;
    connect(allData, &QCheckBox::clicked, this, &ReceiveWindow::clickedAllData);

    auto byteLt = new QGridLayout;
	byteLt->addWidget(new QLabel(tr("Data")), 0, 0);
	byteLt->addWidget(new QLabel(tr("Mask")), 2, 0);

	b1Edit = new BaseSpinBox(16);
	b1Edit->setMaximum(0xFF);
    //connect(b1Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b1Edit, 0, 1);
	byteLt->addWidget(new QLabel(tr("Byte 1")), 1, 1);
	m1Edit = new BaseSpinBox(16);
	m1Edit->setMaximum(0xFF);
    //connect(m1Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m1Edit, 2, 1);

	b2Edit = new BaseSpinBox(16);
	b2Edit->setMaximum(0xFF);
    //connect(b2Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b2Edit, 0, 2);
	byteLt->addWidget(new QLabel(tr("Byte 2")), 1, 2);
	m2Edit = new BaseSpinBox(16);
	m2Edit->setMaximum(0xFF);
    //connect(m2Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m2Edit, 2, 2);

	b3Edit = new BaseSpinBox(16);
	b3Edit->setMaximum(0xFF);
    //connect(b3Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b3Edit, 0, 3);
	byteLt->addWidget(new QLabel(tr("Byte 3")), 1, 3);
	m3Edit = new BaseSpinBox(16);
	m3Edit->setMaximum(0xFF);
    //connect(m3Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m3Edit, 2, 3);

	b4Edit = new BaseSpinBox(16);
	b4Edit->setMaximum(0xFF);
    //connect(b4Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b4Edit, 0, 4);
	byteLt->addWidget(new QLabel(tr("Byte 4")), 1, 4);
	m4Edit = new BaseSpinBox(16);
	m4Edit->setMaximum(0xFF);
    //connect(m4Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m4Edit, 2, 4);

	b5Edit = new BaseSpinBox(16);
	b5Edit->setMaximum(0xFF);
    //connect(b5Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b5Edit, 0, 5);
	byteLt->addWidget(new QLabel(tr("Byte 5")), 1, 5);
	m5Edit = new BaseSpinBox(16);
	m5Edit->setMaximum(0xFF);
    //connect(m5Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m5Edit, 2, 5);

	b6Edit = new BaseSpinBox(16);
	b6Edit->setMaximum(0xFF);
    //connect(b6Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b6Edit, 0, 6);
	byteLt->addWidget(new QLabel(tr("Byte 6")), 1, 6);
	m6Edit = new BaseSpinBox(16);
	m6Edit->setMaximum(0xFF);
    //connect(m6Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m6Edit, 2, 6);

	b7Edit = new BaseSpinBox(16);
	b7Edit->setMaximum(0xFF);
    //connect(b7Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(b7Edit, 0, 7);
	byteLt->addWidget(new QLabel(tr("Byte 7")), 1, 7);
	m7Edit = new BaseSpinBox(16);
	m7Edit->setMaximum(0xFF);
    //connect(m7Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m7Edit, 2, 7);

	b8Edit = new BaseSpinBox(16);
	b8Edit->setMaximum(0xFF);
	byteLt->addWidget(b8Edit, 0, 8);
    //connect(b8Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(new QLabel(tr("Byte 8")), 1, 8);
	m8Edit = new BaseSpinBox(16);
	m8Edit->setMaximum(0xFF);
    //connect(m8Edit, SIGNAL(valueChanged(QString)), this, SLOT(valueChanged(QString)));
	byteLt->addWidget(m8Edit, 2, 8);
	dataLt->addLayout(byteLt);

	dataBox->setLayout(dataLt);
	dataBox->hide();
	layout->addWidget(dataBox);

    box = new QPlainTextEdit;
    box->setFont(getMonospaceFont());
	box->setMinimumSize(600, 300);
    box->setReadOnly(true);
    box->setMaximumBlockCount(MAX_MESSAGES_DISPLAYED);  // increase efficiency
	layout->addWidget(box);

    auto controlLt = new QHBoxLayout;
    auto clearBtn = new QPushButton(tr("Clear"));
    connect(clearBtn, &QPushButton::clicked, box, &QPlainTextEdit::clear);
	controlLt->addWidget(clearBtn);
	controlLt->addStretch(10);
	layout->addLayout(controlLt);

	setLayout(layout);

	// make the window adjust to its layout
	//layout->setSizeConstraint(QLayout::SetMinimumSize);

	if (!connectedDevs.isEmpty())
		deviceChanged(connectedDevs.at(0));
	else
        deviceChanged(nullptr);
}

void ReceiveWindow::deviceConnected(OcDevice *device)
{
    selectDevBox->deviceConnected(device);
	connectedDevs.append(device);

	startBtn->setEnabled(true);
}

void ReceiveWindow::deviceDisconnected(OcDevice *device)
{
    selectDevBox->deviceDisconnected(device);
	connectedDevs.removeAll(device);

	if (connectedDevs.isEmpty())
		startBtn->setEnabled(false);
}

void ReceiveWindow::clickedAllMsgs()
{
	if (allMsgs->isChecked())
        idBox->setVisible(false);
	else
        idBox->setVisible(true);
}

void ReceiveWindow::clickedAllData()
{
	if (allData->isChecked())
        dataBox->setVisible(false);
	else
        dataBox->setVisible(true);
}

void ReceiveWindow::deviceChanged(OcDevice *device)
{
    stopListening();
    selectedDevice = device;
    if (selectedDevice)
	{
        setWindowTitle(PREFIX_RX + selectedDevice->text());

        if (selectedDevice->isExtended())
		{
			idEdit->setMaximum(MAX_EXT);
			idEdit->setValue(MAX_EXT);
			idMaskEdit->setMaximum(MAX_EXT);
			idMaskEdit->setValue(MAX_EXT);
		}
		else
		{
			idEdit->setMaximum(MAX_STD);
			idEdit->setValue(MAX_STD);
			idMaskEdit->setMaximum(MAX_STD);
			idMaskEdit->setValue(MAX_STD);
		}
	}
	else
        setWindowTitle(PREFIX_RX + tr("No Device"));
	if (started)
        startListening();
}

void ReceiveWindow::processMessage(const OcMessage &msg)
{
    if (passFilter(msg))
    {
        QString str;
        QTextStream stream(&str);

        // print the timestamp
        stream << msg.timestamp().toString(Qt::ISODate) << '.';
        stream << left << qSetFieldWidth(3) << qSetPadChar(QChar::fromLatin1('0')) << msg.timestamp().msec();
        stream << "   ";

        stream.setFieldWidth(6);
        stream.setFieldAlignment(QTextStream::AlignRight);
        stream << QString::number(msg.id(), VIEW_BASE).toUpper();

        stream.setFieldWidth(2);

        stream << "   " << QString::number(msg.byte1(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte2(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte3(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte4(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte5(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte6(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte7(), VIEW_BASE).toUpper();
        stream << "   " << QString::number(msg.byte8(), VIEW_BASE).toUpper();

        //box->clear();
        /*if (lines.size() == numViewMsgs)
        {
            for (int x = 0; x < lines.size() - 1; ++x)
            {
                lines[x] = lines[x + 1];
            }
            lines[lines.size() - 1] = str;
        }
        else
            lines.append(str);

        QString boxStr;
        QTextStream boxStream(&boxStr);
        foreach (QString line, lines)
            boxStream << line << endl;
        box->setPlainText(boxStr);*/
        //box->moveCursor(QTextCursor::End);
        box->appendPlainText(str);
    }
}

void ReceiveWindow::start()
{
	if (connectedDevs.isEmpty())
		return;

	if (started)
	{
        stopListening();
		startBtn->setText(tr("Start"));
		started = false;
	}
	else
	{
		started = true;
		startBtn->setText(tr("Stop"));
        startListening();
	}
}

QFont ReceiveWindow::getMonospaceFont()
{
    // Try every option to get a monospace font on any system
    QFont font(QStringLiteral("monospace"));
    if (!QFontInfo(font).fixedPitch())
    {
        font.setStyleHint(QFont::Monospace);
        if (!QFontInfo(font).fixedPitch())
        {
            font.setStyleHint(QFont::TypeWriter);
            if (!QFontInfo(font).fixedPitch())
                font.setFamily(QStringLiteral("courier"));
        }
    }
    return font;
}

void ReceiveWindow::startListening()
{
    if (selectedDevice)
    {
        connect(selectedDevice, &OcDevice::messageReceived, this, &ReceiveWindow::processMessage);
        selectDevBox->setEnabled(false);
    }
}

void ReceiveWindow::stopListening()
{
    if (selectedDevice)
    {
        selectedDevice->disconnect(this);
        selectDevBox->setEnabled(true);
    }
}

bool ReceiveWindow::passFilter(const OcMessage &msg)
{
    if (selectedDevice == nullptr)
		return false;

	quint32 mask = (idEdit->value() & idMaskEdit->value());
	if (allMsgs->isChecked() ||
        (selectedDevice->isExtended() == msg.extended() &&
         ((msg.id() & idMaskEdit->value()) == mask)))
	{
		if (allData->isChecked() || passDataFilter(msg))
			return true;
	}
	return false;
}

bool ReceiveWindow::passDataFilter(const OcMessage &msg)
{
    if ((msg.byte1() & m1Edit->value()) != (b1Edit->value() & m1Edit->value()))
		return false;
    if ((msg.byte2() & m2Edit->value()) != (b2Edit->value() & m2Edit->value()))
		return false;
    if ((msg.byte3() & m3Edit->value()) != (b3Edit->value() & m3Edit->value()))
		return false;
    if ((msg.byte4() & m4Edit->value()) != (b4Edit->value() & m4Edit->value()))
		return false;
    if ((msg.byte5() & m5Edit->value()) != (b5Edit->value() & m5Edit->value()))
		return false;
    if ((msg.byte6() & m6Edit->value()) != (b6Edit->value() & m6Edit->value()))
		return false;
    if ((msg.byte7() & m7Edit->value()) != (b7Edit->value() & m7Edit->value()))
		return false;
    if ((msg.byte8() & m8Edit->value()) != (b8Edit->value() & m8Edit->value()))
		return false;

	return true;
}

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

#include "basespinbox.h"

#include <occore.h>

static const QString HEX_PREFIX = QStringLiteral("0x");
static const QString STR_ZERO = QStringLiteral("0");

BaseSpinBox::BaseSpinBox(int base, QWidget *parent) :
    QSpinBox(parent)
{
	valBase = base;

    connect(this, static_cast<void (QSpinBox::*)(const QString &)>(&QSpinBox::valueChanged), this, &BaseSpinBox::updateBase);
}

QValidator::State BaseSpinBox::validate(QString &text, int &/*pos*/) const
{
	bool ok = false;
    if (text == HEX_PREFIX || text == STR_ZERO)
		return QValidator::Intermediate;
    else if (text.startsWith(HEX_PREFIX))
		text.toInt(&ok, 16);
	else
		text.toInt(&ok, 10);

	if (ok)
		return QValidator::Acceptable;
	else
		return QValidator::Invalid;
}

QString BaseSpinBox::textFromValue(int value) const
{
	QString str = QString::number(value, valBase);
	if (valBase == 16)
        str = HEX_PREFIX + str.toUpper();
	return str;
}

int BaseSpinBox::valueFromText(const QString &text) const
{
	bool ok = false;
    if (text.startsWith(HEX_PREFIX))
		return text.toInt(&ok, 16);
	else
		return text.toInt(&ok, 10);
}

void BaseSpinBox::updateBase(const QString &text)
{
    if (text.startsWith(HEX_PREFIX))
		valBase = 16;
	else
		valBase = 10;
}


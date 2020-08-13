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

#include "plugindialog.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include <ocdevicemanager.h>

using namespace OpenCAN;

static const QString VERSION_NA = QStringLiteral("N/A");

PluginDialog::PluginDialog(OcDeviceManager *manager, QWidget *parent) :
    QDialog(parent)
{
	deviceManager = manager;

	setWindowTitle(tr("Installed Plugins"));

    auto layout = new QVBoxLayout;

	pluginTable = new QTableWidget(0, 3);
	pluginTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Plugin")));
	pluginTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Version")));
	pluginTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Loaded")));
	layout->addWidget(pluginTable);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
									| QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &PluginDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &PluginDialog::reject);
	layout->addWidget(buttonBox);

	setLayout(layout);

	QSize newSize = pluginTable->size() + QSize(layout->spacing() * 2, layout->spacing() * 2);
	newSize.setHeight(height());
	resize(newSize);

	updatePluginList();
}

void PluginDialog::updatePluginList()
{
	pluginTable->clearContents();

	OcPluginInfoList infos = deviceManager->getPluginInfos();
	pluginTable->setRowCount(infos.size());

	int row = 0;
	for (int x = 0; x < infos.size(); ++x)
	{
		OcPluginInfo *info = infos.at(x);

		QString name = info->name();
		if (name.isEmpty())
			name = info->fileName();
		QString version = info->version();
		if (version.isEmpty())
            version = VERSION_NA;

		QTableWidgetItem *nameItem = new QTableWidgetItem(name);
		nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		QTableWidgetItem *versionItem = new QTableWidgetItem(version);
		versionItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        QTableWidgetItem *enabledItem = new QTableWidgetItem(info->status() == Result::Success ? tr("Yes") : tr("No"));
		enabledItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		pluginTable->setItem(row, 0, nameItem);
		pluginTable->setItem(row, 1, versionItem);
		pluginTable->setItem(row, 2, enabledItem);

		++row;
	}
}


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

#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <QMainWindow>

#include <ocdevicemanager.h>

#include "receivewindow.h"

class QAction;
class QMenu;
class QLabel;

class ManagementWindow;

class Testbench : public QMainWindow
{
	Q_OBJECT
public:
	Testbench();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	void aboutOpenCAN();
	void openReceive();
	void openSend();
    void openMultiSend();
	void plugins();

	void deviceConnected(OcDevice *device);
	void deviceDisconnected(OcDevice *device);

private:
	QString connectStatus() const;

	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void readSettings();
	void writeSettings();

	OcDeviceManager deviceMgr;
	OcDeviceList connectedDevs;

	ManagementWindow *manageWindow;
	QLabel *connectStatusLbl;

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *viewMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
    QAction *receiveAct, *sendAct, *multiSendAct, *pluginAct;
	QAction *exitAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *aboutAct, *aboutOcAct, *aboutQtAct;
};

#endif // TESTBENCH_H

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

#include "testbench.h"

#ifdef QT5
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "managementwindow.h"
#include "sendwindow.h"
#include "multisendwindow.h"
#include "plugindialog.h"

static const QString ORG_NAME = QStringLiteral("Code of Intelligence");
static const QString APP_NAME = QStringLiteral("OpenCAN Testbench");
static const QString APP_NAME_SHORT = QStringLiteral("Testbench");
static const QString VERSION = QStringLiteral("0.6");

static const QString NO_DEVICE_TEXT = QObject::tr("Not connected");

static const QString KEY_POSITION = QStringLiteral("pos");
static const QString KEY_SIZE = QStringLiteral("size");

Testbench::Testbench() :
    QMainWindow()
{
    setWindowTitle(APP_NAME + QStringLiteral(" ") + VERSION);

	createActions();
	createMenus();
	//createToolBars();
	createStatusBar();

	deviceMgr.loadPlugins();

	manageWindow = new ManagementWindow(&deviceMgr);
    connect(manageWindow, &ManagementWindow::connected, this, &Testbench::deviceConnected);
    connect(manageWindow, &ManagementWindow::disconnected, this, &Testbench::deviceDisconnected);
	setCentralWidget(manageWindow);

	readSettings();

	// make the window adjust to its layout
	layout()->setSizeConstraint(QLayout::SetFixedSize);

	statusBar()->showMessage(tr("Ready"), 2000);
}

void Testbench::closeEvent(QCloseEvent *)
{
	writeSettings();
	QApplication::quit();
}

void Testbench::about()
{
	QMessageBox::about(this, tr("About ") + APP_NAME,
        QStringLiteral("<p><h3>About ") + APP_NAME + QStringLiteral("</h3>") +
        QStringLiteral("Version ") + VERSION + QStringLiteral("</p>") +
        tr("<p>Testbench is a CAN protocol analyzer, a GUI tool for analyzing and sending messages on a CAN network using the OpenCAN library.") +
		tr("<p>You are free to use and extend Testbench according to the GNU General Public License (GPL) version 3.0.</p>") +
		tr("<p>For more information about Testbench and OpenCAN visit <a href=\"http://www.codeofintelligence.com/opensource/opencan\">www.codeofintelligence.com/opensource/opencan</a>.") +
        QString::fromUtf8("<center><p style=\"font-size: small\">Copyright © 2005-2014 Code of Intelligence, LLC.") +
        tr(" All rights reserved.</p></center>"));
}

void Testbench::aboutOpenCAN()
{
	QMessageBox::about(this, QObject::tr("About OpenCAN"), OpenCAN_About());
}

void Testbench::createActions()
{
    exitAct = new QAction(QIcon(QStringLiteral(":/exit.png")), tr("&Quit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Quit ") + APP_NAME_SHORT);
    connect(exitAct, &QAction::triggered, this, &Testbench::close);

    cutAct = new QAction(QIcon(QStringLiteral(":/editcut.png")), tr("Cu&t"), this);
	cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    //connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(QStringLiteral(":/editcopy.png")), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    //connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(QStringLiteral(":/editpaste.png")), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    //connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

	receiveAct = new QAction(tr("New Receive Window"), this);
	receiveAct->setStatusTip(tr("Open a new receive window"));
    connect(receiveAct, &QAction::triggered, this, &Testbench::openReceive);

	sendAct = new QAction(tr("New Send Message Window"), this);
	sendAct->setStatusTip(tr("Open a new send message window"));
    connect(sendAct, &QAction::triggered, this, &Testbench::openSend);

    multiSendAct = new QAction(tr("New Multi Send Message Window"), this);
    multiSendAct->setStatusTip(tr("Open a new send multiple message window"));
    connect(multiSendAct, &QAction::triggered, this, &Testbench::openMultiSend);

	pluginAct = new QAction(tr("Plugins..."), this);
	pluginAct->setStatusTip(tr("View installed OpenCAN plugins"));
    connect(pluginAct, &QAction::triggered, this, &Testbench::plugins);
      
	aboutAct = new QAction(tr("&About ") + APP_NAME, this);
	aboutAct->setStatusTip(tr("Show ") + APP_NAME_SHORT + tr("'s About box"));
    aboutAct->setMenuRole(QAction::AboutRole);
    connect(aboutAct, &QAction::triggered, this, &Testbench::about);

	aboutOcAct = new QAction(tr("About &OpenCAN"), this);
	aboutOcAct->setStatusTip(tr("Show the OpenCAN library's About box"));
    aboutOcAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(aboutOcAct, &QAction::triggered, this, &Testbench::aboutOpenCAN);

    aboutQtAct = new QAction(tr("About Qt"), this);
    aboutQtAct->setStatusTip(tr("Show Qt library's About box"));
    aboutQtAct->setMenuRole(QAction::AboutQtRole);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

	cutAct->setEnabled(false);
	copyAct->setEnabled(false);

#ifdef Q_OS_MAC
    // icons look odd in Mac OS X system menu
    exitAct->setIconVisibleInMenu(false);
    cutAct->setIconVisibleInMenu(false);
    copyAct->setIconVisibleInMenu(false);
    pasteAct->setIconVisibleInMenu(false);
#endif
}

void Testbench::createMenus()
{
    // Mac OS X unified title and toolbar look - introduced in Qt 5.2
#if (QT_VERSION >= QT_VERSION_CHECK(5, 2, 0))
    setUnifiedTitleAndToolBarOnMac(true);
#endif

    // create new parentless menu bar
    // on Mac, this will create one menu for all windows
    setMenuBar(new QMenuBar);

    fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(receiveAct);
	viewMenu->addAction(sendAct);
    viewMenu->addAction(multiSendAct);
	viewMenu->addSeparator();
	viewMenu->addAction(pluginAct);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutOcAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAct);
}

void Testbench::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
}

void Testbench::createStatusBar()
{
	connectStatusLbl = new QLabel(connectStatus());
	statusBar()->addPermanentWidget(connectStatusLbl);
}

void Testbench::readSettings()
{
	QSettings settings(ORG_NAME, APP_NAME);
    QPoint pos = settings.value(KEY_POSITION, QPoint(200, 200)).toPoint();
    QSize size = settings.value(KEY_SIZE, QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void Testbench::writeSettings()
{
	QSettings settings(ORG_NAME, APP_NAME);
    settings.setValue(KEY_POSITION, pos());
    settings.setValue(KEY_SIZE, size());
}

void Testbench::openReceive()
{
	ReceiveWindow *wnd = new ReceiveWindow(connectedDevs);
    connect(manageWindow, &ManagementWindow::connected, wnd, &ReceiveWindow::deviceConnected);
    connect(manageWindow, &ManagementWindow::disconnected, wnd, &ReceiveWindow::deviceDisconnected);
	wnd->show();
}

void Testbench::openSend()
{
	SendWindow *wnd = new SendWindow(connectedDevs);
    connect(manageWindow, &ManagementWindow::connected, wnd, &SendWindow::deviceConnected);
    connect(manageWindow, &ManagementWindow::disconnected, wnd, &SendWindow::deviceDisconnected);
	wnd->show();
}

void Testbench::openMultiSend()
{
    MultiSendWindow *wnd = new MultiSendWindow(connectedDevs);
    connect(manageWindow, &ManagementWindow::connected, wnd, &MultiSendWindow::deviceConnected);
    connect(manageWindow, &ManagementWindow::disconnected, wnd, &MultiSendWindow::deviceDisconnected);
    wnd->show();
}

void Testbench::plugins()
{
	PluginDialog dlg(&deviceMgr);
	dlg.exec();
}

void Testbench::deviceConnected(OcDevice *device)
{
	if (!connectedDevs.contains(device))
	{
		connectedDevs.append(device);
		connectStatusLbl->setText(connectStatus());
	}
}

void Testbench::deviceDisconnected(OcDevice *device)
{
	connectedDevs.removeAll(device);
	connectStatusLbl->setText(connectStatus());
}

QString Testbench::connectStatus() const
{
	if (connectedDevs.isEmpty())
		return NO_DEVICE_TEXT;
	else
	{
		QString text = tr("Connected to ") + QString::number(connectedDevs.size());
		if (connectedDevs.size() > 1)
			text += tr(" devices");
		else
			text += tr(" device");
		return text;
	}
}

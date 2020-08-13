/******************************************************************************
**
**  Copyright (C) 2005-2020 Code of Intelligence, LLC.
**  Contact: contact@opencan.org
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public
**  License version 2.1 as published by the Free Software Foundation and
**  appearing in the file LICENSE included in the packaging of this file.
**  Please review the following information to ensure the GNU Lesser General
**  Public License version 2.1 requirements will be met:
**  http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**  Commercial License Usage
**  Licensees holding valid commercial OpenCAN licenses may use this file in
**  accordance with the commercial license agreement or other written agreement
**  between you and Code of Intelligence that governs your use of the Software.
**  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#include <ocdevicemanager.h>

using namespace OpenCAN;

// Main documentation
#include "ocdoc.h"  // TODO: Make sure this is added to documentation at beginning

#include <QtCore>

static const QString OPENCAN_VERSION_QSTR = QStringLiteral(OPENCAN_VERSION_STR);

static const QString MAC_DIR_NAME = QStringLiteral("MacOS");
static const QString PLUGINS_DIR = QStringLiteral("ocplugins");

#if defined(Q_OS_WIN)
    static const QString PLUGIN_EXT = QStringLiteral("*.dll");
#elif defined(Q_OS_MACX)
    static const QString PLUGIN_EXT = QStringLiteral("*.dylib");
#else
    static const QString PLUGIN_EXT = QStringLiteral("*.so");
#endif

// Global functions
QString OpenCAN_Version()
{
    return OPENCAN_VERSION_QSTR;
}

QString OpenCAN_About()
{
	return QObject::tr("<p><h3>About OpenCAN</h3>") +
        QObject::tr("This software uses OpenCAN version ") + OpenCAN_Version() + QStringLiteral(".</p>") +
		QObject::tr("<p>OpenCAN is a software platform for interacting with various Controller Area Network (CAN) devices.</p>") +
		QObject::tr("<p>OpenCAN provides an abstract C++ interface that can be used to control any CAN device, and each component is cross-platform, enabling the efficient development of CAN software on Windows, Linux and Mac OS X.</p>") +
		QObject::tr("<p>You are free to use and extend OpenCAN according to the GNU Lesser General Public License (LGPL) version 2.1.</p>") +
		QObject::tr("OpenCAN is a Code of Intelligence product. For more information visit <a href=\"http://www.codeofintelligence.com/opensource/opencan\">www.codeofintelligence.com/opensource/opencan</a>.") +
        QString::fromUtf8("<center><p style=\"font-size: small\">Copyright © 2005-2014 Code of Intelligence, LLC.</p></center>");
}

// OcDeviceManager

OcDeviceManager::OcDeviceManager()
{
    qDebug() << QStringLiteral("OpenCAN version %1").arg(OPENCAN_VERSION_QSTR);
    qDebug();
}

OcDeviceManager::~OcDeviceManager()
{
    foreach (OcPluginInfo *info, pluginInfos)
	{
        if (info->status() == Result::Failure)	// no plugin loaded
			delete info;
	}
}

/// Get the current linked version of OpenCAN
/// @returns the version as a QString
QString OcDeviceManager::version()
{
    return OPENCAN_VERSION_QSTR;
}

/// Load static and dynamically-loaded CAN device plugins
/// Looks in the ocplugins directory in the application's working directory
void OcDeviceManager::loadPlugins()
{
    qDebug() << QStringLiteral("Loading static OpenCAN device plugins");

	// static plugins
	foreach (QObject *plugin, QPluginLoader::staticInstances())
		loadDeviceInterface(qobject_cast<OcDeviceInterface *>(plugin));

	// dynamically-loaded plugins

    // from application directory
    QDir pluginsDir = QDir::currentPath();
#if defined(Q_OS_MAC)
	if (pluginsDir.dirName() == MAC_DIR_NAME)
	{
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif
	pluginsDir.cd(PLUGINS_DIR);
    loadPlugins(pluginsDir);

    // from global directory
#if defined(Q_OS_WIN) | defined(Q_OS_LINUX)
    loadPlugins(QDir(QStringLiteral(OPENCAN_DIR)));
#endif
}

/// Get a CAN device
/// @param type the type of CAN device<br/>
/// Examples are <em>lincan</em>, etc.
/// An empty string uses the first available type
/// @param device the name of the device
/// @returns nullptr if no device interfaces have been loaded, a device does not
/// exist, or an interface matching type is not loaded.
OcDevice* OcDeviceManager::getDevice(const QString &type, const QString &device)
{
	if (!plugins.empty() && plugins.value(type))
        return plugins[type]->getDevice(device);

    return nullptr;
}

void OcDeviceManager::loadPlugins(const QDir &dir)
{
    qDebug() << QStringLiteral("Loading OpenCAN device plugins from ") + dir.absolutePath();
    foreach (QString fileName, dir.entryList(QStringList() << PLUGIN_EXT))
    {
        QString filePath = dir.absoluteFilePath(fileName);
        qDebug() << filePath;

        QPluginLoader loader(filePath);
        QObject *plugin = loader.instance();
        if (plugin)
        {
            loadDeviceInterface(qobject_cast<OcDeviceInterface *>(plugin), filePath);
        }
        else
        {
            qDebug() << loader.errorString();
            OcPluginInfo *info = new OcPluginInfo();
            info->setFileName(filePath);
            pluginInfos.append(info);
        }
    }
}

void OcDeviceManager::loadDeviceInterface(OcDeviceInterface *interface, const QString &fileName)
{
	if (interface)
	{
		plugins[interface->type()] = interface;

        OcPluginInfo *info = interface->info();
		info->setFileName(fileName);
		pluginInfos.append(info);

		interface->discoverDevices();
		devices.append(interface->getDevices());
	}
	else
	{
        qDebug() << QStringLiteral("OcDeviceManager::loadDeviceInterface: nullptr");
        qDebug() << QStringLiteral("Unable to load plugin %1").arg(fileName);
		OcPluginInfo *info = new OcPluginInfo();
		info->setFileName(fileName);
		pluginInfos.append(info);
	}
}

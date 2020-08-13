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

#ifndef OCPLUGININFO_H
#define OCPLUGININFO_H

#include <QStringList>

#include <occore.h>

OPENCAN_HEADER_BEGIN

/// OpenCAN plugin information

/// Only name() and version() are guaranteed to be non-empty
class OPENCAN_EXPORT OcPluginInfo
{
public:
    OcPluginInfo() : devStatus(OpenCAN::Result::Failure) {}
	OcPluginInfo(const QString &name, const QString &type,
                 const QString &version, OpenCAN::Result status)
		: devName(name), devType(type), pluginVersion(version), devStatus(status) {}
    virtual ~OcPluginInfo() = default;

	/// The name of the OpenCAN driver
    QString name() const { return devName; }

	/// OpenCAN device type string
	/// The type string is used to get devices from the OcDeviceManager
	/// using OcDeviceManager::getDevice()
    QString type() const { return devType; }

	/// The version of the OpenCAN driver
    QString version() const { return pluginVersion; }

	/// The filename of the OpenCAN plugin
	/// @returns the filename otherwise an empty string if the plugin is statically linked
    QString fileName() const { return file; }

	/// Get the status of the plugin
	/// @returns OcSuccess if the plugin was loaded successfully, otherwise OcFail
    OpenCAN::Result status() const { return devStatus; }

	/// The person or organization who created the OpenCAN driver
    QString vendor() const { return pluginVendor; }

	/// The person or organization who created the device
    QString deviceVendor() const { return devVendor; }

	/// The version of the device hardware
    QString deviceVersion() const { return devVersion; }

	/// Device hardware description
    QString deviceDescription() const { return devDesc; }

	/// The hardware device driver version
    QString deviceDriverVersion() const { return devDrvVersion; }

	/// The person or organization who created the hardware device driver
    QString deviceDriverVendor() const { return devDrvVendor; }

	/// OpenCAN driver copyright
    QString copyright() const { return copy; }

	/// OpenCAN driver license
    QString license() const { return lic; }

	/// Any OpenCAN driver dependencies as a list
    QStringList dependencies() const { return deps; }

	/// Set the plugin's file name
	/// Note: The file name should be empty if statically linked
    void setFileName(const QString &fileName) { file = fileName; }

	/// Set the name of the person or organization who created the OpenCAN driver
    void setVendor(const QString &vendor) { pluginVendor = vendor; }

	/// Set the name of the person or organization who created the device
    void setDeviceVendor(const QString &vendor) { devVendor = vendor; }

	/// Set the version of the device hardware
    void setDeviceVersion(const QString &version) { devVersion = version; }

	/// Set the description of the device hardware
    void setDeviceDescription(const QString &description) { devDesc = description; }

	/// Set the version of the hardware device driver
    void setDeviceDriverVersion(const QString &version) { devDrvVersion = version; }

    /// Set the name of the person or organization who created the hardware device driver
    void setDeviceDriverVendor(const QString &vendor) { devDrvVendor = vendor; }

	/// Set the OpenCAN driver copyright
    void setCopyright(const QString &copyright) { copy = copyright; }

	/// Set the OpenCAN driver license
    void setLicense(const QString &license) { lic = license; }

	/// Add a dependency to the list of dependencies
    void addDependency(const QString &dependency) { deps.append(dependency); }

	/// Remove a dependency from the list of dependencies
    void removeDependency(int index) { deps.removeAt(index); }

	/// Set the list of dependencies
    void setDependencies(const QStringList &dependencies) { deps = dependencies; }
	// TODO: Should be able to set a range of compatible/dependent versions with optional end

private:
	QString devName, devType, pluginVersion, file, pluginVendor, devVendor,
			devVersion, devDesc, devDrvVersion, devDrvVendor, copy, lic;
	QStringList deps;
    OpenCAN::Result devStatus;
};

OPENCAN_HEADER_END

#endif // OCPLUGININFO_H

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

#include <ocmessage.h>

/// Create an empty CAN message.
/// The length of the message data is set to 0.
/// @param extended true if the message uses extended CAN
OcMessage::OcMessage(bool extended)
{
	msgId = 0;
	dataLen = 0;
	canExt = extended;
    std::fill_n(msgData, MAX_MESSAGE_LENGTH, 0);
}

/// Create a CAN message with data given by an array of 8 bytes.
/// @param id the message identifier
/// @param data an array of 8 bytes containing the message data
/// @param length the length of the message's data
/// @param extended true if the message uses extended CAN
/// @param timestamp the time the message was received
/*OcMessage::OcMessage(quint32 id, quint8 data[MAX_MESSAGE_LENGTH], int length, bool extended, const QTime &timestamp)
{
	msgId = id;
	dataLen = length;
	canExt = extended;
	time = timestamp;
    std::copy(data, &data[length], msgData);
}*/

/// Create a CAN message with data given 8 separate bytes.
/// @param id the message identifier
/// @param byte1 the first data byte
/// @param byte2 the second data byte
/// @param byte3 the third data byte
/// @param byte4 the fourth data byte
/// @param byte5 the fifth data byte
/// @param byte6 the sixth data byte
/// @param byte7 the seventh data byte
/// @param byte8 the eighth data byte
/// @param extended true if the message uses extended CAN
/// /// @param timestamp the time the message was received
OcMessage::OcMessage(quint32 id, quint8 byte1, quint8 byte2, quint8 byte3,
					 quint8 byte4, quint8 byte5, quint8 byte6, quint8 byte7,
                     quint8 byte8, int length, bool extended, const QTime &timestamp)
{
	msgId = id;
	canExt = extended;
	time = timestamp;
	dataLen = length;
	msgData[0] = byte1;
	msgData[1] = byte2;
	msgData[2] = byte3;
	msgData[3] = byte4;
	msgData[4] = byte5;
	msgData[5] = byte6;
	msgData[6] = byte7;
	msgData[7] = byte8;
}

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

#ifndef OCMESSAGE_H
#define OCMESSAGE_H

#include <QTime>

#include <occore.h>

OPENCAN_HEADER_BEGIN

static const int MAX_MESSAGE_LENGTH = 8;

/// Encapsulates a CAN message.

/// Each OcMessage stores a timestamp that is set when the message is received
/// or can be set with setTimestamp() for other messages.
class OPENCAN_EXPORT OcMessage
{
public:
    explicit OcMessage(bool extended = true);
    /*OcMessage(quint32 id, quint8 data[MAX_MESSAGE_LENGTH],
              int length = MAX_MESSAGE_LENGTH, bool extended = true, const QTime &timestamp = QTime());*/
    OcMessage(quint32 id, quint8 data[MAX_MESSAGE_LENGTH], int length = MAX_MESSAGE_LENGTH,
              bool extended = true, const QTime &timestamp = QTime())
        : OcMessage{id, data[0], data[1], data[2], data[3], data[4], data[5],
                    data[6], data[7], length, extended, timestamp} {}
	OcMessage(quint32 id, quint8 byte0 = 0, quint8 byte1 = 0, quint8 byte2 = 0,
			  quint8 byte3 = 0, quint8 byte4 = 0, quint8 byte5 = 0,
			  quint8 byte6 = 0, quint8 byte7 = 0,
              int length = MAX_MESSAGE_LENGTH, bool extended = true, const QTime &timestamp = QTime());
    virtual ~OcMessage() = default;

    /// Copy constructor
	OcMessage(const OcMessage &msg)
	{
		msgId = msg.msgId;
		canExt = msg.canExt;
		dataLen = msg.dataLen;
        time = msg.time;
        std::copy(msg.msgData, &msg.msgData[MAX_MESSAGE_LENGTH], msgData);
	}

    /// Assignment operator
    OcMessage& operator=(const OcMessage &msg)
    {
        msgId = msg.msgId;
        canExt = msg.canExt;
        dataLen = msg.dataLen;
        time = msg.time;
        std::copy(msg.msgData, &msg.msgData[MAX_MESSAGE_LENGTH], msgData);
        return *this;
    }

	quint32 id() const;
	bool extended() const;
    const QTime& timestamp() const;
	int length() const;
	quint8 byte1() const;
	quint8 byte2() const;
	quint8 byte3() const;
	quint8 byte4() const;
	quint8 byte5() const;
	quint8 byte6() const;
	quint8 byte7() const;
	quint8 byte8() const;
	const quint8* data() const;
	quint8* data();

	void setExtended(bool extended);
	void setId(quint32 id);
    void setTimestamp(const QTime &timestamp);
	void setData(quint8 data[MAX_MESSAGE_LENGTH], int length = MAX_MESSAGE_LENGTH);

	bool operator==(const OcMessage &m) const;

protected:
	/// CAN message identifier
	quint32 msgId;
	/// CAN message data
	quint8 msgData[MAX_MESSAGE_LENGTH];
	/// Length of CAN message data
	int dataLen;
	/// Boolean value for whether message follows CAN 2.0b extended
	bool canExt;
    /// Timestamp
    QTime time;
};

// Inline OcMessage Methods

/// Get the message identifier
inline quint32 OcMessage::id() const { return msgId; }

/// Get whether the message follows CAN 2.0b extended
inline bool OcMessage::extended() const { return canExt; }

/// Get the time the message was received.
/// The returned QTime object may return true to isNull() if
/// the timestamp was not sent, either by the OcDevice when it
/// was received or by calling setTimestamp().
inline const QTime& OcMessage::timestamp() const { return time; }

/// Get the length of the message's data in bytes
inline int OcMessage::length() const { return dataLen; }

/// Get the first data byte
inline quint8 OcMessage::byte1() const { return msgData[0]; }

/// Get the second data byte
inline quint8 OcMessage::byte2() const { return msgData[1]; }

/// Get the third data byte
inline quint8 OcMessage::byte3() const { return msgData[2]; }

/// Get the fourth data byte
inline quint8 OcMessage::byte4() const { return msgData[3]; }

/// Get the fifth data byte
inline quint8 OcMessage::byte5() const { return msgData[4]; }

/// Get the sixth data byte
inline quint8 OcMessage::byte6() const { return msgData[5]; }

/// Get the seventh data byte
inline quint8 OcMessage::byte7() const { return msgData[6]; }

/// Get the eighth data byte
inline quint8 OcMessage::byte8() const { return msgData[7]; }

/// Get a const pointer to the data array
inline const quint8* OcMessage::data() const { return msgData; }

/// Get a pointer to the data array
inline quint8* OcMessage::data() { return msgData; }

/// Set the message identifier
/// @param id the message identifier
inline void OcMessage::setId(quint32 id) { msgId = id; }

/// Set whether the message follows CAN 2.0b extended
/// @param extended true if the message follows CAN 2.0b extended
inline void OcMessage::setExtended(bool extended) { canExt = extended; }

/// Set the message's timestamp
/// @param timestamp the timestamp
inline void OcMessage::setTimestamp(const QTime &timestamp) { time = timestamp; }

/// Set the message data
/// @param data the message data
/// @param length the length of the data in bytes
inline void OcMessage::setData(quint8 data[MAX_MESSAGE_LENGTH], int length)
{
	dataLen = length;
    std::copy(data, &data[length], msgData);
}

/// Verify if two messages contain the same content, ignoring the timestamp.
/// Compares message identifier, data, data length, and extended status.
/// This method does not compare the timestamp on purpose, so that messages
/// containing the same message identifier and data will return true even though
/// they may have been received at different times.
/// @param m the message to compare this message to
/// @returns true if the two messages match
inline bool OcMessage::operator==(const OcMessage &m) const
{
	return (canExt == m.canExt && msgId == m.msgId && dataLen == m.dataLen &&
		msgData[0] == m.msgData[0] && msgData[1] == msgData[1] && msgData[2] == m.msgData[2] &&
		msgData[3] == m.msgData[3] && msgData[4] == m.msgData[4] && msgData[5] == m.msgData[5] &&
        msgData[6] == m.msgData[6] && msgData[7] == m.msgData[7]);
}

OPENCAN_HEADER_END

#endif // OCMESSAGE_H

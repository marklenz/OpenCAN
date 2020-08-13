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
**  Licensees holding valid commercial OpenCAN and SimCAN licenses may use
**  this file in accordance with the commercial license agreement or other
**  written agreement between you and Code of Intelligence that governs your
**  use of the Software.  For further information, use the contact form at
**  http://codeofintelligence.com/contact.
**
******************************************************************************/

#ifndef OCSIMCANMESSAGEBUFFER_H
#define OCSIMCANMESSAGEBUFFER_H

#include <QSharedMemory>

#include "occore.h"

static const int NUM_MESSAGES_IN_BUFFER = 20;

struct Message
{
    quint32 deviceID;
    quint32 id;
    quint8 dataLength;
    quint8 data[8];
};

struct MessageBuffer
{
    quint8 *lastWrittenIndex;    // last index written, 0xFF if no messages have been written yet
    Message (*messages)[NUM_MESSAGES_IN_BUFFER];
};

// NOTE: If a SimCAN device cannot keep up with the buffer,
// unread messages will be overwritten. I.e., when the
// lastIndex written rolls over and becomes greater than
// lastReadIndex, the device could miss the entire width
// of the buffer in messages.
//
// Write timestamp to buffer and print a message if the time between when a message is written and read is greater than a threshold

class OcSimCANMessageBuffer : public QSharedMemory
{
    Q_OBJECT
public:
    explicit OcSimCANMessageBuffer(const QString &deviceNum, QObject *parent = 0);

    bool open();
    bool close();
    bool write(const Message &msg);
    bool newMessage();
    bool read(Message &msg);

private:
    void setupBuffer();

    MessageBuffer buffer;

    quint32 deviceID;
    quint8 lastReadIndex;
};

#endif // OCSIMCANMESSAGEBUFFER_H

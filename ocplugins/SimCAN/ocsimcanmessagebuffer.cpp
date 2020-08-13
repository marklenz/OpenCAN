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

#include "ocsimcanmessagebuffer.h"

#include <QDebug>

#include <ocmessage.h>

#include <cstdlib>
#include <ctime>

static const int BYTES_HEADER = 1;  // 1 byte to hold next message address
static const int BYTES_MESSAGE = 17;    // 4 bytes device ID, 4 bytes message ID, 1 byte data length, and 8 bytes data
static const int INIT_HEADER = 0xFF;  // init header to this to flag no messages have been written

// Constants for QSharedMemory
static const QString IPC_KEY = QStringLiteral("/canipc");
static const int SHM_SIZE = BYTES_HEADER + (NUM_MESSAGES_IN_BUFFER * BYTES_MESSAGE);

OcSimCANMessageBuffer::OcSimCANMessageBuffer(const QString &deviceNum, QObject *parent) :
    QSharedMemory(IPC_KEY + deviceNum, parent)
{
    srand(time(nullptr));
    deviceID = rand() % 0xFFFFFFFF + 1;
    lastReadIndex = INIT_HEADER;
}

bool OcSimCANMessageBuffer::open()
{
    if (isAttached())
        return false;

    // try to create the shared memory
    bool retval = true;
    if (!create(SHM_SIZE))
    {
        switch (error())
        {
        case QSharedMemory::AlreadyExists:
            // try to attach to the existing shared memory
            if (attach())
            {
                setupBuffer();

                // set lastReadIndex to lastWrittenIndex in buffer
                // so device will only read newer messages
                lastReadIndex = *(buffer.lastWrittenIndex);
            }
            else
            {
                qDebug() << errorString();
                retval = false;
            }
            break;
        default:
            qDebug() << errorString();
            retval = false;
            break;
        }
    }
    else
    {
        setupBuffer();

        // initialize shared memory message buffer
        if (lock())
        {
            *(buffer.lastWrittenIndex) = INIT_HEADER;
            unlock();
        }
        else
        {
            qDebug() << errorString();
            retval = false;
        }
    }

    return retval;
}

bool OcSimCANMessageBuffer::close()
{
    lastReadIndex = INIT_HEADER;

    if (!isAttached())
        return false;

    return detach();
}

bool OcSimCANMessageBuffer::write(const Message &msg)
{
    if (!isAttached())
        return false;

    if (lock())
    {
        // increment lastWrittenIndex to the index of the message about to be written
        // incrementing INIT_HEADER (0xFF) in a quint8 will result in 0, right
        // where we want to start, otherwise this will continue moving through
        // the buffer
        *(buffer.lastWrittenIndex) = (*(buffer.lastWrittenIndex) + 1) % NUM_MESSAGES_IN_BUFFER;

        // store message in shared memory buffer
        (*(buffer.messages))[*(buffer.lastWrittenIndex)].deviceID = deviceID;
        (*(buffer.messages))[*(buffer.lastWrittenIndex)].id = msg.id;

        std::copy(msg.data, &msg.data[MAX_MESSAGE_LENGTH],
                  (*(buffer.messages))[*(buffer.lastWrittenIndex)].data);

        unlock();
        return true;
    }

    return false;
}

bool OcSimCANMessageBuffer::newMessage()
{
    if (!isAttached())
        return false;

    bool retval = false;
    if (lock())
    {
        // nothing new to read if lastWrittenIndex == lastReadIndex
        if (*(buffer.lastWrittenIndex) != lastReadIndex)
            retval = true;
        unlock();
    }

    return retval;
}

bool OcSimCANMessageBuffer::read(Message &msg)
{
    if (!isAttached())
        return false;

    if (lock())
    {
        // check if new messages have been written
        bool newMessage = false;
        if (*(buffer.lastWrittenIndex) != lastReadIndex)
        {
            // increment lastReadIndex to the index of the message about to be read
            // incrementing INIT_HEADER (0xFF) in a quint8 will result in 0, right
            // where we want to start, otherwise this will continue moving through
            // the buffer
            lastReadIndex = (lastReadIndex + 1) % NUM_MESSAGES_IN_BUFFER;

            if ((*(buffer.messages))[lastReadIndex].deviceID != deviceID) // don't read message sent from this device
            {
                // load message from shared memory buffer
                msg.id = (*(buffer.messages))[lastReadIndex].id;

                std::copy((*(buffer.messages))[lastReadIndex].data,
                          &(*(buffer.messages))[lastReadIndex].data[MAX_MESSAGE_LENGTH],
                          msg.data);

                newMessage = true;
            }
        }

        unlock();
        return newMessage;
    }

    return false;
}

void OcSimCANMessageBuffer::setupBuffer()
{
    // set the memory structure pointers to their address
    // in the shared memory
    quint8 *memory = (quint8 *)data();
    buffer.lastWrittenIndex = memory;
    buffer.messages = (Message (*)[NUM_MESSAGES_IN_BUFFER])(memory + 1);
}

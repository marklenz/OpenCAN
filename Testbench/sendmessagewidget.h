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

#ifndef SENDMESSAGEWIDGET_H
#define SENDMESSAGEWIDGET_H

#include <QWidget>

class QSpinBox;
class QCheckBox;
class QPushButton;

class BaseSpinBox;
class OcMessage;

class SendMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SendMessageWidget(QWidget *parent = nullptr);

signals:
    void sendMessage(OcMessage &message);

private slots:
    void sendMessageSlot();
    void timerChanged(int t);
    void enableSendChanged(int state);

private:
    void startTimer();

    BaseSpinBox *msgId;
    BaseSpinBox *byte1, *byte2, *byte3, *byte4, *byte5, *byte6, *byte7, *byte8;
    QSpinBox *sendTime;
    QCheckBox *enableSend;
    QPushButton *sendMsg;
    QTimer *sendTimer;
};

#endif // SENDMESSAGEWIDGET_H

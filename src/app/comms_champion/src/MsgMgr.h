//
// Copyright 2014 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include <memory>
#include <vector>

#include <QtCore/QObject>

#include "comms_champion/Message.h"
#include "comms_champion/Protocol.h"
#include "comms_champion/MessageInfo.h"
#include "comms_champion/Socket.h"

namespace comms_champion
{

class MsgMgr : public QObject
{
    Q_OBJECT

    typedef QObject Base;
public:
    typedef unsigned long long MsgNumberType;

    typedef std::vector<MessageInfoPtr> MsgsList;

    typedef DataInfo::Timestamp Timestamp;

    enum class MsgType {
        Received,
        Sent
    };


    static MsgMgr* instance();
    static MsgMgr& instanceRef();

    ~MsgMgr();

    void start();
    void stop();
    void clear();

    ProtocolPtr getProtocol() const;
    void setRecvEnabled(bool enabled);

    void deleteMsg(MessageInfoPtr msgInfo);
    void deleteAllMsgs();

    void sendMsgs(const MsgInfosList& msgs);

    const MsgsList& getAllMsgs() const;

public slots:
    void addSocket(SocketPtr socket);
    void removeSocket(SocketPtr socket);
    void setProtocol(ProtocolPtr protocol);

signals:
    void sigMsgAdded(MessageInfoPtr msgInfo);
    void sigErrorReported(const QString& msg);

private slots:
    void socketDataReceived(DataInfoPtr dataInfoPtr);


private:
    typedef std::list<SocketPtr> SocketsList;

    MsgMgr(QObject* parent = nullptr);
    void updateInternalId(MessageInfo& msgInfo);

    MsgsList m_allMsgs;
    bool m_recvEnabled = false;

    SocketsList m_sockets;
    ProtocolPtr m_protocol;
    MsgNumberType m_nextMsgNum = 1;
    bool m_running = false;
};

}  // namespace comms_champion
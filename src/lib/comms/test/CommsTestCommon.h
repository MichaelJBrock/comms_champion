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

#include "tuple"

#include "comms/comms.h"

enum MessageType {
    MessageType1,
    UnusedValue1,
    MessageType2,
    UnusedValue2,
    UnusedValue3,
    MessageType3
};

template <typename TTraits>
class TestMessageBase : public comms::Message<TTraits>
{
    typedef comms::Message<TTraits> Base;
public:
    using Base::Message;

    virtual ~TestMessageBase() {}

    const std::string& getName() const
    {
        return this->getNameImpl();
    }

private:
    virtual const std::string& getNameImpl() const = 0;
};


template <typename TField>
using FieldsMessage1 =
    std::tuple<
        comms::field::BasicIntValue<TField, std::uint16_t>
    >;

template <typename TMessage>
class Message1 : public
        comms::MessageBase<
            TMessage,
            comms::option::NumIdImpl<MessageType1>,
            comms::option::FieldsImpl<FieldsMessage1<typename TMessage::Field> >,
            comms::option::DispatchImpl<Message1<TMessage> >
        >
{
public:

    Message1() = default;

    virtual ~Message1() = default;

protected:

    virtual const std::string& getNameImpl() const
    {
        static const std::string str("Message1");
        return str;
    }
};

template <typename... TArgs>
bool operator==(
    const Message1<TArgs...>& msg1,
    const Message1<TArgs...>& msg2)
{
    return msg1.getFields() == msg2.getFields();
}

template <typename TMessage>
class Message2 : public
    comms::MessageBase<
        TMessage,
        comms::option::NumIdImpl<MessageType2>,
        comms::option::NoFieldsImpl,
        comms::option::DispatchImpl<Message2<TMessage> >
    >
{
public:
    virtual ~Message2() = default;

protected:
    virtual const std::string& getNameImpl() const
    {
        static const std::string str("Message2");
        return str;
    }
};

template <typename... TArgs>
bool operator==(
    const Message2<TArgs...>& msg1,
    const Message2<TArgs...>& msg2)
{
    return msg1.getFields() == msg2.getFields();
}

template <typename TField>
using Message3Fields =
    std::tuple<
        comms::field::BasicIntValue<TField, std::uint32_t>,
        comms::field::BasicIntValue<
            TField,
            std::int16_t,
            comms::field::option::LengthLimitImpl<1>,
            comms::field::option::ValidRangeImpl<-200, 200>,
            comms::field::option::DefaultValueImpl<255> >, // invalid upon creation
        comms::field::BitmaskValue<TField, 2>,
        comms::field::BitmaskValue<TField, 3>
    >;

template <typename TMessage>
class Message3 : public
    comms::MessageBase<
        TMessage,
        comms::option::NumIdImpl<MessageType3>,
        comms::option::FieldsImpl<Message3Fields<typename TMessage::Field> >,
        comms::option::DispatchImpl<Message3<TMessage> >
    >
{
public:
    Message3() = default;

    virtual ~Message3() = default;

protected:

    virtual const std::string& getNameImpl() const
    {
        static const std::string str("Message3");
        return str;
    }
};

template <typename... TArgs>
bool operator==(
    const Message3<TArgs...>& msg1,
    const Message3<TArgs...>& msg2)
{
    return msg1.getFields() == msg2.getFields();
}

template <typename TMessage>
using AllMessages =
    std::tuple<
        Message1<TMessage>,
        Message2<TMessage>,
        Message3<TMessage>
    >;


template <typename TProtStack>
typename TProtStack::MsgPtr commonReadWriteMsgTest(
    TProtStack& stack,
    const char* const buf,
    std::size_t bufSize,
    comms::ErrorStatus expectedEs = comms::ErrorStatus::Success)
{
    typedef typename TProtStack::MsgPtr MsgPtr;

    MsgPtr msg;
    auto readIter = buf;
    auto es = stack.read(msg, readIter, bufSize);
    TS_ASSERT_EQUALS(es, expectedEs);
    if (es != comms::ErrorStatus::Success) {
        return std::move(msg);
    }

    TS_ASSERT(msg);

    auto actualBufSize = static_cast<std::size_t>(std::distance(buf, readIter));
    TS_ASSERT_EQUALS(actualBufSize, stack.length(*msg));
    std::unique_ptr<char []> outCheckBuf(new char[actualBufSize]);
    auto writeIter = &outCheckBuf[0];
    es = stack.write(*msg, writeIter, actualBufSize);
    TS_ASSERT_EQUALS(es, comms::ErrorStatus::Success);
    TS_ASSERT(std::equal(buf, buf + actualBufSize, &outCheckBuf[0]));
    return std::move(msg);
}

template <typename TProtStack, typename TMessage>
void commonWriteReadMsgTest(
    TProtStack& stack,
    TMessage msg,
    char* buf,
    std::size_t bufSize,
    const char* expectedBuf,
    comms::ErrorStatus expectedEs = comms::ErrorStatus::Success)
{
    auto writeIter = buf;
    auto es = stack.write(msg, writeIter, bufSize);
    TS_ASSERT_EQUALS(es, expectedEs);
    if (es != comms::ErrorStatus::Success) {
        return;
    }

    assert(expectedBuf != nullptr);
    TS_ASSERT(std::equal(buf, buf + bufSize, &expectedBuf[0]));

    typedef typename TProtStack::MsgPtr MsgPtr;
    MsgPtr msgPtr;
    auto readIter = expectedBuf;
    es = stack.read(msgPtr, readIter, bufSize);
    TS_ASSERT_EQUALS(es, comms::ErrorStatus::Success);
    TS_ASSERT(msgPtr);
    TS_ASSERT_EQUALS(msgPtr->getId(), msg.getId());
    auto* castedMsg = dynamic_cast<TMessage*>(msgPtr.get());
    TS_ASSERT(castedMsg != nullptr);
    TS_ASSERT_EQUALS(*castedMsg, msg);
}

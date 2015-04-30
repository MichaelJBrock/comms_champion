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

#include <cstdint>
#include <cstddef>

#include <QtCore/QString>

namespace comms_champion
{

namespace field_wrapper
{

class FieldWrapper
{
public:
    typedef std::vector<std::uint8_t> SerialisedSeq;

    virtual ~FieldWrapper() {};

    std::size_t length() const
    {
        return lengthImpl();
    }

    int width() const
    {
        return static_cast<int>(length()) * 2;
    }

    bool valid() const
    {
        return validImpl();
    }

    SerialisedSeq getSerialisedValue() const
    {
        return getSerialisedValueImpl();
    }

    bool setSerialisedValue(const SerialisedSeq& value)
    {
        return setSerialisedValueImpl(value);
    }

    QString getSerialisedString() const;

    bool setSerialisedString(const QString& str);

protected:
    virtual std::size_t lengthImpl() const = 0;
    virtual bool validImpl() const = 0;
    virtual SerialisedSeq getSerialisedValueImpl() const = 0;
    virtual bool setSerialisedValueImpl(const SerialisedSeq& value) = 0;
};

template <typename TBase, typename TField>
class FieldWrapperT : public TBase
{
    using Base = TBase;
    using Field = TField;
public:
    virtual ~FieldWrapperT() = default;

protected:

    FieldWrapperT(Field& field)
      : m_field(field)
    {
    }

    virtual std::size_t lengthImpl() const override
    {
        return m_field.length();
    }

    virtual bool validImpl() const override
    {
        return m_field.valid();
    }

    Field& field()
    {
        return m_field;
    }

    const Field& field() const
    {
        return m_field;
    }


private:
    Field& m_field;
};

}  // namespace field_wrapper

}  // namespace comms_champion

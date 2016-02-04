// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#include "plug.h"


Plug::Plug(const PlugDefinition &definition, QObject *parent) :
    QObject(parent),
    _definition(definition),
    _value()
{
}

const PlugDefinition &Plug::getDefinition() const
{
    return _definition;
}

void Plug::setValue(const QVariant &value)
{
    if(value != _value)
    {
        _value = value;
        emit valueChanged(_value);
    }
}

const QVariant &Plug::getValue() const
{
    return _value;
}

void Plug::signalConnectedTo(const Plug *connectedTo)
{
    emit connectionChanged(connectedTo);
}

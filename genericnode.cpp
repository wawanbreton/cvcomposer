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

#include "genericnode.h"

#include <QDebug>
#include <QEvent>
#include <QVariant>

#include "processors/abstractprocessor.h"
#include "plug.h"


GenericNode::GenericNode(const QString &name,
                         const QString &userReadableName,
                         QObject *parent) :
    QObject(parent),
    _name(name),
    _userReadableName(userReadableName),
    _inputs(),
    _outputs()
{
    QMetaType processorType(QMetaType::type((name + "Processor").toUtf8()));
    if(processorType.isValid())
    {
         AbstractProcessor *processor = static_cast<AbstractProcessor *>(processorType.create());

         foreach(const PlugDefinition input, processor->getInputs())
         {
             _inputs << new Plug(input, this);
         }
         foreach(const PlugDefinition output, processor->getOutputs())
         {
             _outputs << new Plug(output, this);
         }

         delete processor;
    }
    else
    {
        qCritical() << "GenericNode::GenericNode" << "Unable to find processor for" << name;
    }
}

const QString &GenericNode::getName() const
{
    return _name;
}

const QString &GenericNode::getUserReadableName() const
{
    return _userReadableName;
}

quint8 GenericNode::getNbInputs() const
{
    return _inputs.count();
}

const QList<Plug *> &GenericNode::getInputs() const
{
    return _inputs;
}

bool GenericNode::hasInput(Plug *input) const
{
    return _inputs.contains(input);
}

quint8 GenericNode::getNbOutputs() const
{
    return _outputs.count();
}

const QList<Plug *> &GenericNode::getOutputs() const
{
    return _outputs;
}

bool GenericNode::hasOutput(Plug *output) const
{
    return _outputs.contains(output);
}

void GenericNode::signalProcessDone(const Properties &outputs, const Properties &inputs)
{
    emit processDone(outputs, inputs);
}

void GenericNode::signalProcessUnavailable()
{
    emit processUnavailable();
}

const Properties &GenericNode::getProperties() const
{
    return _properties;
}

void GenericNode::setProperties(const Properties &properties)
{
    _properties = properties;
}

void GenericNode::setProperty(const QString &name, const QVariant &value)
{
    qDebug() << "GenericNode::setProperty" << name << value;
    if(value != _properties.value(name))
    {
        _properties[name] = value;
        emit propertyChanged(name, value);
    }
}

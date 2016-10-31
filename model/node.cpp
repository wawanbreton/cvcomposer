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

#include "node.h"

#include <QDebug>
#include <QEvent>
#include <QVariant>

#include "processor/processorsfactory.h"
#include "processor/abstractprocessor.h"
#include "model/plug.h"


Node::Node(const QString &name,
                         const QString &userReadableName,
                         QObject *parent) :
    QObject(parent),
    _name(name),
    _userReadableName(userReadableName),
    _inputs(),
    _outputs()
{
    AbstractProcessor *processor = ProcessorsFactory::createProcessor(name);
    if(processor)
    {
        foreach(const PlugDefinition input, processor->getInputs())
        {
            _inputs << new Plug(input, this);
            _properties.insert(input.name, input.defaultValue);
        }
        foreach(const PlugDefinition output, processor->getOutputs())
        {
            _outputs << new Plug(output, this);
            _properties.insert(output.name, output.defaultValue);
        }

        delete processor;
    }
}

const QString &Node::getName() const
{
    return _name;
}

const QString &Node::getUserReadableName() const
{
    return _userReadableName;
}

const QList<Plug *> &Node::getInputs() const
{
    return _inputs;
}

bool Node::hasInput(const Plug *input) const
{
    return _inputs.contains((Plug *)input);
}

Plug *Node::findInput(const QString &name) const
{
    for(Plug *plug : _inputs)
    {
        if(plug->getDefinition().name == name)
        {
            return plug;
        }
    }

    return NULL;
}

const QList<Plug *> &Node::getOutputs() const
{
    return _outputs;
}

bool Node::hasOutput(const Plug *output) const
{
    return _outputs.contains((Plug *)output);
}

Plug *Node::findOutput(const QString &name) const
{
    for(Plug *plug : _outputs)
    {
        if(plug->getDefinition().name == name)
        {
            return plug;
        }
    }

    return NULL;
}

const Properties &Node::getProperties() const
{
    return _properties;
}

void Node::setProperty(const QString &name, const QVariant &value)
{
    if(value != _properties.value(name))
    {
        _properties[name] = value;
        emit propertyChanged(name, value);
    }
}

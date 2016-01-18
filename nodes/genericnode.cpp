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

#include <QEvent>
#include <QVariant>


GenericNode::GenericNode(const QString &name,
                         const QString &userReadableName,
                         quint8 nbInputs,
                         quint8 nbOutputs,
                         QObject *parent) :
    QObject(parent),
    _name(name),
    _userReadableName(userReadableName),
    _inputs(),
    _outputs()
{
    _inputs.reserve(nbInputs);
    for(quint8 i = 0 ; i < nbInputs ; i++)
    {
        _inputs << new Plug(this);
    }

    for(quint8 i = 0 ; i < nbOutputs ; i++)
    {
        _outputs << new Plug(this);
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

bool GenericNode::event(QEvent *event)
{
    if(event->type() == QEvent::DynamicPropertyChange)
    {
        QDynamicPropertyChangeEvent *changeEvent =
                static_cast<QDynamicPropertyChangeEvent *>(event);
        if(not changeEvent->propertyName().startsWith("_q_"))
        {
            emit propertyChanged(changeEvent->propertyName(),
                                 property(changeEvent->propertyName()));
        }
    }

    return QObject::event(event);
}


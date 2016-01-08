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

#include "composermodel.h"

#include <QDebug>

#include "nodes/abstractnode.h"


ComposerModel::ComposerModel(QObject *parent) :
    QObject(parent),
    _nodes()
{

}

void ComposerModel::addNode(AbstractNode *node)
{
    _nodes << node;
    node->setParent(this);
}

AbstractNode *ComposerModel::findInputPlug(const QUuid &plugId) const
{
    return findPlug(plugId, true, false);
}

AbstractNode *ComposerModel::findOutputPlug(const QUuid &plugId) const
{
    return findPlug(plugId, false, true);
}

AbstractNode *ComposerModel::findPlug(const QUuid &plugId, bool fromInputs, bool fromOutputs) const
{
    foreach(AbstractNode *node, _nodes)
    {
        if(fromInputs && node->hasInput(plugId))
        {
            return node;
        }
        if(fromOutputs && node->hasOutput(plugId))
        {
            return node;
        }
    }

    return NULL;
}

QUuid ComposerModel::addConnection(const QUuid &output, const QUuid &input)
{
    #warning Do appropriate checks

    QMutableMapIterator<QUuid, Connection> iterator(_connections);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.value().input == input)
        {
            QUuid removedKey = iterator.key();
            iterator.remove();
            emit connectionRemoved(removedKey);
        }
    }

    QUuid connectionId = QUuid::createUuid();
    Connection connection;
    connection.output = output;
    connection.input = input;
    _connections.insert(connectionId, connection);
    emit connectionAdded(connectionId);

    return connectionId;
}

void ComposerModel::removeConnection(const QUuid &connectionId)
{
    if(_connections.remove(connectionId))
    {
        emit connectionRemoved(connectionId);
    }
    else
    {
        qWarning() << "ComposerModel::removeConnection" << "No connection with ID" << connectionId;
    }
}

Connection ComposerModel::getConnection(const QUuid &connectionId) const
{
    return _connections.value(connectionId);
}

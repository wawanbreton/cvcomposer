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
#include <QThread>

#include "execution/composerscheduler.h"
#include "model/connection.h"
#include "model/node.h"


ComposerModel::ComposerModel(QObject *parent) :
    QObject(parent),
    _nodes(),
    _connections()
{
}

void ComposerModel::addNode(Node *node)
{
    _nodes << node;
    node->setParent(this);

    emit nodeAdded(node);
}

QList<const Node *> ComposerModel::getNodes() const
{
    QList<const Node *> nodes;

    foreach(const Node *node, _nodes)
    {
        nodes << node;
    }

    return nodes;
}

void ComposerModel::removeNode(Node *node)
{
    foreach(Connection *connection, _connections)
    {
        if(node->getInputs().contains(connection->getInput()) ||
           node->getOutputs().contains(connection->getOutput()))
        {
            removeConnection(connection);
        }
    }

    if(_nodes.removeAll(node))
    {
        emit nodeRemoved(node);
        delete node;
    }
    else
    {
        qCritical() << "Removing a not registered node ?!";
    }
}

Node *ComposerModel::findInputPlug(const Plug *plug) const
{
    return findPlug(plug, true, false);
}

Node *ComposerModel::findOutputPlug(const Plug *plug) const
{
    return findPlug(plug, false, true);
}

Node *ComposerModel::findPlug(const Plug *plug, bool fromInputs, bool fromOutputs) const
{
    foreach(Node *node, _nodes)
    {
        if(fromInputs && node->hasInput(plug))
        {
            return node;
        }
        if(fromOutputs && node->hasOutput(plug))
        {
            return node;
        }
    }

    return NULL;
}

const Connection *ComposerModel::findConnectionToInput(const Plug *input) const
{
    foreach(const Connection *connection, _connections)
    {
        if(connection->getInput() == input)
        {
            return connection;
        }
    }

    return NULL;
}

QSet<const Node *> ComposerModel::findDirectDescendantNodes(const Plug *output) const
{
    QSet<const Node *> nodes;

    foreach(const Connection *connection, _connections)
    {
        if(connection->getOutput() == output)
        {
            const Node *descendantNode = findInputPlug(connection->getInput());
            if(descendantNode)
            {
                nodes += findDescendantNodes(descendantNode);
            }
            else
            {
                qCritical() << "A connection has no valid node";
            }
        }
    }

    return nodes;
}

QSet<const Node *> ComposerModel::findDescendantNodes(const Node *node, bool includeParent) const
{
    QSet<const Node *> nodes;

    if(includeParent)
    {
        nodes << node;
    }

    for(const Plug *output : node->getOutputs())
    {
        for(const Node *descendantNode : findDirectDescendantNodes(output))
        {
            nodes += findDescendantNodes(descendantNode);
        }
    }

    return nodes;
}

void ComposerModel::addConnection(Plug *output, Plug *input)
{
    #warning Do appropriate checks

    QMutableListIterator<Connection *> iterator(_connections);
    while(iterator.hasNext())
    {
        iterator.next();
        if(iterator.value()->getInput() == input)
        {
            Connection *connection = iterator.value();
            connection->getInput()->signalConnectedTo(NULL);
            connection->getOutput()->signalConnectedTo(NULL);
            iterator.remove();
            delete connection;
            emit connectionRemoved(connection);
        }
    }

    Connection *connection = new Connection(output, input, this);
    _connections << connection;
    input->signalConnectedTo(output);
    output->signalConnectedTo(input);
    emit connectionAdded(connection);
}

void ComposerModel::removeConnection(const Connection *connection)
{
    if(_connections.removeAll((Connection *)connection))
    {
        connection->getInput()->signalConnectedTo(NULL);
        connection->getOutput()->signalConnectedTo(NULL);
        emit connectionRemoved(connection);
        delete connection;
    }
    else
    {
        qWarning() << "ComposerModel::removeConnection No such connection";
    }
}

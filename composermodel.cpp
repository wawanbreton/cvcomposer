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

#include "composerscheduler.h"
#include "connection.h"
#include "nodes/genericnode.h"


ComposerModel::ComposerModel(QObject *parent) :
    QObject(parent),
    _nodes(),
    _connections(),
    _scheduler(NULL)
{

}

void ComposerModel::addNode(GenericNode *node)
{
    _nodes << node;
    node->setParent(this);
    connect(node, SIGNAL(propertyChanged(QString,QVariant)), SLOT(startExecution()));
}

GenericNode *ComposerModel::findInputPlug(Plug *plug) const
{
    return findPlug(plug, true, false);
}

GenericNode *ComposerModel::findOutputPlug(Plug *plug) const
{
    return findPlug(plug, false, true);
}

GenericNode *ComposerModel::findPlug(Plug *plug, bool fromInputs, bool fromOutputs) const
{
    foreach(GenericNode *node, _nodes)
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
            iterator.remove();
            delete connection;
            emit connectionRemoved(connection);
        }
    }

    Connection *connection = new Connection(output, input, this);
    _connections << connection;
    emit connectionAdded(connection);

    startExecution();
}

void ComposerModel::removeConnection(Connection *connection)
{
    if(_connections.removeAll(connection))
    {
        emit connectionRemoved(connection);
        delete connection;

        startExecution();
    }
    else
    {
        qWarning() << "ComposerModel::removeConnection No such connection";
    }
}

void ComposerModel::startExecution()
{
    if(not _scheduler.isNull())
    {

    }

    _scheduler = new ComposerScheduler(this);
    _scheduler->execute(_nodes, _connections);
}

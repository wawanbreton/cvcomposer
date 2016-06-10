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
    _connections(),
    _executorSettings(),
    _scheduler(NULL)
{
    _executorSettings.cacheData = true;
    _executorSettings.useMultiThreading = true;
    _executorSettings.useOptimalThreadsCount = true;
    _executorSettings.fixedThreadsCount = QThread::idealThreadCount();
}

void ComposerModel::addNode(Node *node)
{
    _nodes << node;
    node->setParent(this);
    connect(node, SIGNAL(propertyChanged(QString,QVariant)), SLOT(startExecution()));
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

    startExecution();
}

void ComposerModel::removeConnection(Connection *connection)
{
    if(_connections.removeAll(connection))
    {
        connection->getInput()->signalConnectedTo(NULL);
        connection->getOutput()->signalConnectedTo(NULL);
        emit connectionRemoved(connection);
        delete connection;

        startExecution();
    }
    else
    {
        qWarning() << "ComposerModel::removeConnection No such connection";
    }
}

const ExecutorSettings &ComposerModel::getExecutorSettings() const
{
    return _executorSettings;
}

void ComposerModel::setExecutorSettings(const ExecutorSettings &settings)
{
    _executorSettings = settings;
    startExecution();
}

void ComposerModel::startExecution()
{
    ComposerScheduler *newScheduler = new ComposerScheduler(this);
    newScheduler->prepareExecution(_nodes, _connections);

    if(not _scheduler.isNull())
    {
        _scheduler->cancel();
        connect(_scheduler.data(), SIGNAL(destroyed()), newScheduler, SLOT(execute()));
    }
    else
    {
        newScheduler->execute();
    }

    _scheduler = newScheduler;
}

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

#include "composerscheduler.h"

#include <QDebug>

#include "connection.h"
#include "genericnode.h"
#include "composerexecutor.h"


ComposerScheduler::ComposerScheduler(QObject *parent) :
    QObject(parent),
    _executor(new ComposerExecutor(this)),
    _executionList(),
    _connections(),
    _unreachableNodes(),
    _processedNodes(),
    _cancelled(false)
{
    connect(_executor, SIGNAL(nodeProcessed(bool, Properties)),
                       SLOT(onNodeProcessed(bool, Properties)));
}

void ComposerScheduler::prepareExecution(const QList<GenericNode *> &nodes,
                                         const QList<Connection *> &connections)
{
    _connections = connections;

    _processedNodes.clear();
    _executionList.clear();

    QList<GenericNode *> nodesToProcess = nodes;

    do
    {
        QMutableListIterator<GenericNode *> iterator(nodesToProcess);
        while(iterator.hasNext())
        {
            iterator.next();
            GenericNode *nodeToProcess = iterator.value();

            // For each remaining node, check whether all its inputs are available, i.e. we can
            // process it now

            bool allInputsProcessed = true;
            foreach(Plug *input, nodeToProcess->getInputs())
            {
                if(PlugType::isInputPluggable(input->getDefinition().type) == PlugType::ManualOnly)
                {
                    // Plug can't be connected, so it is always valid
                    continue;
                }

                // Find the connection to this input
                const Connection *connection = findConnectionToInput(input);

                // We have found the connection, now find the previous node
                GenericNode *previousNode = NULL;
                if(connection)
                {
                    foreach(GenericNode *node, nodes)
                    {
                        if(node->hasOutput(connection->getOutput()))
                        {
                            previousNode = node;
                            break;
                        }
                    }
                }

                if(_unreachableNodes.contains(previousNode) ||
                   (previousNode == NULL && PlugType::isInputPluggable(input->getDefinition().type) == PlugType::Mandatory))
                {
                    // Previous node is unreachable or mandatory input is not connected,
                    // there is no way we can process the node
                    _unreachableNodes << iterator.value();
                    iterator.remove();
                    allInputsProcessed = false;
                    break; // Don't bother checking other plugs
                }
                else if(previousNode != NULL && not _executionList.contains(previousNode))
                {
                    // Output of previous node has not been processed yet
                    allInputsProcessed = false;
                    break; // Don't bother checking other plugs
                }
            }

            if(allInputsProcessed)
            {
                // All inputs of node have been processed, we can process it now !
                _executionList.enqueue(nodeToProcess);
                iterator.remove();
            }
        }
    } while(not nodesToProcess.isEmpty());
}

void ComposerScheduler::cancel()
{
    // Just tag the scheduler as cancelled, and wait for an execute() or onNodeProcessed() slot call
    _cancelled = true;
}

void ComposerScheduler::execute()
{
    if(_cancelled)
    {
        // Scheduler was cancelled even before it started :(
        deleteLater();
        return;
    }

    foreach(GenericNode *node, _unreachableNodes)
    {
        node->signalProcessUnavailable();
    }

    processNextIfPossible();
}

void ComposerScheduler::onNodeProcessed(bool success, const Properties &outputs)
{
    if(_cancelled)
    {
        deleteLater();
        return;
    }

    GenericNode *processedNode = _executionList.dequeue();
    if(success)
    {
        _processedNodes[processedNode] = outputs;
    }

    processNextIfPossible();
}

bool ComposerScheduler::makeInputs(GenericNode *node, Properties &inputs)
{
    foreach(Plug *plug, node->getInputs())
    {
        QString plugName = plug->getDefinition().name;

        // Find whether this plug is connected
        const Connection *connection = findConnectionToInput(plug);
        if(connection)
        {
            // This plug is connected, get the output value of the previous node, if available
            bool nodeProcessed = false;
            QMapIterator<GenericNode *, Properties> iterator(_processedNodes);
            while(iterator.hasNext())
            {
                iterator.next();

                if(iterator.key()->hasOutput(connection->getOutput()))
                {
                    // The node providing the output has been processed, great !
                    nodeProcessed = true;
                    QString outputName = connection->getOutput()->getDefinition().name;
                    inputs.insert(plugName, iterator.value()[outputName]);
                }
            }

            if(not nodeProcessed)
            {
                // The input node couldn't be processed :(
                return false;
            }
        }
        else
        {
            // This plus is not connected, use the user-defined value
            inputs.insert(plugName, node->getProperties()[plugName]);
        }
    }

    return true;
}

void ComposerScheduler::processNextIfPossible()
{
    if(_executionList.isEmpty())
    {
        deleteLater();
    }
    else
    {
        Properties inputs;
        if(makeInputs(_executionList.head(), inputs))
        {
            _executor->process(_executionList.head(), inputs);
        }
        else
        {
            onNodeProcessed(false, Properties());
        }
    }
}

const Connection *ComposerScheduler::findConnectionToInput(const Plug *input)
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

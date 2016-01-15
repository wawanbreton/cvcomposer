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
#include "nodes/abstractnode.h"
#include "composerexecutor.h"


ComposerScheduler::ComposerScheduler(QObject *parent) :
    QObject(parent),
    _executor(new ComposerExecutor(this))
{
    connect(_executor, SIGNAL(nodeProcessed(bool, QList<cv::Mat>)),
                       SLOT(onNodeProcessed(bool, QList<cv::Mat>)));
}

void ComposerScheduler::execute(const QList<AbstractNode *> &nodes,
                                const QList<Connection *> &connections)
{
    QList<AbstractNode *> pseudoProcessedNodes;
    QList<AbstractNode *> nodesToProcess = nodes;
    QList<AbstractNode *> unreachableNodes;
    _executionList.clear();

    do
    {
        QMutableListIterator<AbstractNode *> iterator(nodesToProcess);
        while(iterator.hasNext())
        {
            iterator.next();
            AbstractNode *nodeToProcess = iterator.value();
            QList<AbstractNode *> dependancies;

            // For each remaining node, check whether all its inputs are available, i.e. we can
            // process it now

            bool allInputsProcessed = true;
            foreach(Plug *input, nodeToProcess->getInputs())
            {
                // Find the connection to this input
                const Connection *connection = NULL;
                foreach(const Connection *aConnection, connections)
                {
                    if(aConnection->getInput() == input)
                    {
                        connection = aConnection;
                        break;
                    }
                }

                // We have found the connection, now find the previous node
                AbstractNode *previousNode = NULL;
                if(connection)
                {
                    foreach(AbstractNode *node, nodes)
                    {
                        if(node->hasOutput(connection->getOutput()))
                        {
                            previousNode = node;
                            break;
                        }
                    }
                }

                if(previousNode == NULL || unreachableNodes.contains(previousNode))
                {
                    // Input is not connected or previous node is unreachable, there is no way
                    // we can process the node
                    unreachableNodes << iterator.value();
                    iterator.remove();
                    allInputsProcessed = false;
                    break; // Don't bother checking other plugs

                }
                else if(pseudoProcessedNodes.contains(previousNode))
                {
                    // Output of previous node has been processed
                    dependancies << previousNode;
                }
                else
                {
                    allInputsProcessed = false;
                    break; // Don't bother checking other plugs
                }
            }

            if(allInputsProcessed)
            {
                // All inputs of node have been processed, we can process it now !
                _executionList.enqueue(qMakePair(nodeToProcess, dependancies));
                pseudoProcessedNodes << nodeToProcess;
                iterator.remove();
            }
        }
    } while(not nodesToProcess.isEmpty());

    if(not _executionList.isEmpty())
    {
        _executor->processNode(_executionList.head().first, QList<cv::Mat>());
    }

    unreachableNodes << nodesToProcess;
    foreach(AbstractNode *node, unreachableNodes)
    {
        node->signalProcessUnavailable();
    }
}

void ComposerScheduler::onNodeProcessed(bool success, const QList<cv::Mat> &outputs)
{
    QPair<AbstractNode *, QList<AbstractNode *> > processedNode = _executionList.dequeue();
    if(success)
    {
        _processedNodes[processedNode.first] = outputs;
    }
    else
    {
        // One node execution has failed, now find all the nodes dependant of it
        bool nodeRemoved;
        QList<AbstractNode *> removedNodes;
        removedNodes << processedNode.first;
        do
        {
            nodeRemoved = false;
            QMutableListIterator<QPair<AbstractNode *, QList<AbstractNode *> > > iterator(_executionList);
            while(iterator.hasNext())
            {
                iterator.next();

                foreach(AbstractNode *removedNode, removedNodes)
                {
                    if(iterator.value().second.contains(removedNode))
                    {
                        // This node has a removed dependancy, tag is as unavailable and add it to
                        // the list so that the nodes dependant of it become unavailable too
                        iterator.value().first->signalProcessUnavailable();
                        nodeRemoved = true;
                        removedNodes << iterator.value().first;
                        iterator.remove();
                        break;
                    }
                }
            }
        } while(nodeRemoved);
    }

    if(not _executionList.isEmpty())
    {
        QList<cv::Mat> inputs;
        foreach(AbstractNode *dependancy, _executionList.head().second)
        {
            #warning Dependancies should be sorted in inputs order (for nodes with multiple inputs)
            inputs << _processedNodes[dependancy];
        }

        _executor->processNode(_executionList.head().first, inputs);
    }
}

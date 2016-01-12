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
    connect(_executor, SIGNAL(nodeProcessed(QList<cv::Mat>)),
                       SLOT(onNodeProcessed(QList<cv::Mat>)));
}

void ComposerScheduler::execute(const QList<AbstractNode *> &nodes,
                                const QList<Connection *> &connections)
{
    QList<AbstractNode *> pseudoProcessedNodes;
    QList<AbstractNode *> previousPseudoProcessedNodes;
    QList<AbstractNode *> nodesToProcess = nodes;
    QList<AbstractNode *> unreachableNodes;
    _executionList.clear();

    do
    {
        previousPseudoProcessedNodes = pseudoProcessedNodes;

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
                bool connectionFound = false;
                foreach(const Connection *connection, connections)
                {
                    if(connection->getInput() == input)
                    {
                        connectionFound = true;

                        // We have found the connection, now check whether the output of the other
                        // node is processed
                        bool outputProcessed = false;
                        foreach(AbstractNode *node, pseudoProcessedNodes)
                        {
                            if(node->hasOutput(connection->getOutput()))
                            {
                                // Output of previous node has been processed
                                outputProcessed = true;
                                dependancies << node;
                            }
                        }

                        allInputsProcessed &= outputProcessed;
                    }
                }

                if(not connectionFound)
                {
                    // Input is not connected, there is no way we can process the node
                    unreachableNodes << iterator.value();
                    iterator.remove();
                    allInputsProcessed = false;
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
    } while(not nodesToProcess.isEmpty() && pseudoProcessedNodes != previousPseudoProcessedNodes);

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

void ComposerScheduler::onNodeProcessed(const QList<cv::Mat> &outputs)
{
    QPair<AbstractNode *, QList<AbstractNode *> > processedNode = _executionList.dequeue();
    _processedNodes[processedNode.first] = outputs;

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

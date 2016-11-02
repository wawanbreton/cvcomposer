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

#include "execution/composerscheduler.h"

#include <QDebug>
#include <QTimer>

#include "model/composermodel.h"
#include "model/connection.h"
#include "model/node.h"
#include "composerexecutor.h"


ComposerScheduler::ComposerScheduler(const ComposerModel *model, QObject *parent) :
    QObject(parent),
    _currentExecutors(),
    _oldExecutors(),
    _keepProcessingNodes(),
    _model(model),
    _end(false)
{
    _settings.cacheData = true;
    _settings.useMultiThreading = true;
    _settings.useOptimalThreadsCount = true;
    _settings.fixedThreadsCount = QThread::idealThreadCount();
}

void ComposerScheduler::start()
{
    connect(_model, SIGNAL(nodeAdded(const Node*)), SLOT(onNodeAdded(const Node*)));
    connect(_model, SIGNAL(nodeRemoved(const Node*)), SLOT(onNodeRemoved(const Node*)));
    connect(_model, SIGNAL(connectionAdded(const Connection*)),
                    SLOT(onConnectionAdded(const Connection*)));
    connect(_model, SIGNAL(connectionRemoved(const Connection*)),
                    SLOT(onConnectionRemoved(const Connection*)));

    for(const Node *node : _model->getNodes())
    {
        onNodeAdded(node);
    }

    processNexts();
}

void ComposerScheduler::end()
{
    if(_currentExecutors.isEmpty() && _oldExecutors.isEmpty())
    {
        // There is no executor being processed, exit asap
        QTimer::singleShot(0, this, SIGNAL(ended()));
    }
    else
    {
        // There are remaining executors, wait for their end before exiting
        _end = true;
    }
}

void ComposerScheduler::setSettings(const ExecutorSettings &settings)
{
    bool reprocess = settings.cacheData != _settings.cacheData;

    _settings = settings;

    if(reprocess)
    {
        reProcessAll();
    }
}

const ExecutorSettings &ComposerScheduler::getSettings() const
{
    return _settings;
}

void ComposerScheduler::onNodeAdded(const Node *node)
{
    connect(node, SIGNAL(propertyChanged(QString,QVariant)), SLOT(onNodePropertyChanged()));

    if(allInputsProcessed(node))
    {
        // If the node only has free plugs, process it ASAP
        processNexts();
    }
}

void ComposerScheduler::onNodeRemoved(const Node *node)
{
    // Invalidate potential executors currently running for this node
    cancelExecutors(node);

    // Untag the node as needeing a reprocess
    _keepProcessingNodes.removeAll(node);

    // Removed the cached data for this node
    _processedNodes.remove(node);
}

void ComposerScheduler::onNodePropertyChanged()
{
    if(_settings.cacheData)
    {
        const Node *node = qobject_cast<const Node *>(sender());
        if(node)
        {
            reProcessFromNode(node);
        }
        else
        {
            qCritical() << "Sender is not a Node instance";
        }
    }
    else
    {
        reProcessAll();
    }
}

void ComposerScheduler::onConnectionRemoved(const Connection *connection)
{
    if(_settings.cacheData)
    {
        const Node *descendantNode = _model->findInputPlug(connection->getInput());
        if(descendantNode)
        {
            PlugType::PlugTypes inputTypes = connection->getInput()->getDefinition().types;
            if(PlugType::isInputPluggable(inputTypes) == PlugType::Mandatory)
            {
                // The descendant node is no more valid, and so are all its descendants
                invalidateFromNode(descendantNode);
            }
            else
            {
                // The descendant node is to be recomputed now with the manually-entered value
                reProcessFromNode(descendantNode);
            }
        }
        else
        {
            qCritical() << "Unable to find descendant node";
        }
    }
    else
    {
        reProcessAll();
    }
}

void ComposerScheduler::onConnectionAdded(const Connection *connection)
{
    if(_settings.cacheData)
    {
        const Node *descendantNode = _model->findInputPlug(connection->getInput());
        if(descendantNode)
        {
            reProcessFromNode(descendantNode);
        }
        else
        {
            qCritical() << "Unable to find descendant node";
        }
    }
    else
    {
        reProcessAll();
    }
}

void ComposerScheduler::onNodeProcessed()
{
    ComposerExecutor *executor = qobject_cast<ComposerExecutor *>(sender());
    if(executor)
    {
        if(_end)
        {
            // Execution has been asked to end, give up
            _currentExecutors.removeAll(executor);
            _oldExecutors.removeAll(executor);

            end();
        }
        else
        {
            bool current = _currentExecutors.contains(executor);
            bool old = _oldExecutors.contains(executor);

            if(current && old)
            {
                qCritical() << "Executor is registered as both current and old ?!";
            }
            if(!current && !old)
            {
                qCritical() << "Executor is not registered as current nor old ?!";
            }

            if(current)
            {
                // The executor is still awaited
                if(executor->getKeepProcessing())
                {
                    _keepProcessingNodes << executor->getNode();
                }

                emit executorEnded(executor->getNode(),
                                   executor->getOutputs(),
                                   executor->getInputs(),
                                   executor->getDuration(),
                                   executor->getError());

                if(executor->getError().isEmpty())
                {
                    _processedNodes[executor->getNode()] = executor->getOutputs();
                }
                else
                {
                    _processedNodes[executor->getNode()] = Properties();
                    invalidateFromNode(executor->getNode(), false);
                }

                if(!_settings.cacheData)
                {
                    clearUnusedCache();
                }

                _currentExecutors.removeAll(executor);
            }
            else
            {
                // This is an old executor, just unregister it and keep processing
                _oldExecutors.removeAll(executor);
            }

            processNexts();
        }
    }
    else
    {
        qCritical() << "Sender is not a ComposerExecutor instance";
    }
}

void ComposerScheduler::onProgress(qreal progress)
{
    ComposerExecutor *executor = qobject_cast<ComposerExecutor *>(sender());
    if(executor)
    {
        emit executorProgress(executor->getNode(), progress);
    }
    else
    {
        qCritical() << "Sender is not a ComposerExecutor instance";
    }
}

bool ComposerScheduler::allInputsProcessed(const Node *node)
{
    foreach(Plug *input, node->getInputs())
    {
        if(PlugType::isInputPluggable(input->getDefinition().types) == PlugType::ManualOnly)
        {
            // Plug can't be connected, so it is always valid
            continue;
        }

        // Find the connection to this input
        const Connection *connection = _model->findConnectionToInput(input);

        // We have found the connection, now find the previous node
        Node *previousNode = NULL;
        if(connection)
        {
            previousNode = _model->findOutputPlug(connection->getOutput());
        }

        if(previousNode == NULL)
        {
            if(PlugType::isInputPluggable(input->getDefinition().types) == PlugType::Mandatory)
            {
                // Plug is not connected and it should be
                return false;
            }
            else
            {
                // Plug is not connected, but this is authorized
            }
        }
        else if(!_processedNodes.contains(previousNode))
        {
            // Plug is connected, but its input node has not been processed yet
            return false;
        }
    }

    return true;
}

void ComposerScheduler::makeInputs(const Node *node, Properties &inputs)
{
    foreach(Plug *plug, node->getInputs())
    {
        QString plugName = plug->getDefinition().name;

        // Find whether this plug is connected
        const Connection *connection = _model->findConnectionToInput(plug);
        if(connection)
        {
            // This plug is connected, get the output value of the previous node, if available
            bool nodeProcessed = false;
            QMapIterator<const Node *, Properties> iterator(_processedNodes);
            while(iterator.hasNext())
            {
                iterator.next();

                if(iterator.key()->hasOutput(connection->getOutput()))
                {
                    // The node providing the output has been processed, great !
                    nodeProcessed = true;
                    QString outputName = connection->getOutput()->getDefinition().name;
                    inputs.insert(plugName, iterator.value().value(outputName));
                }
            }

            if(not nodeProcessed)
            {
                qCritical() << "Inputs for node" << node->getName()
                            << "are asked but at least one input node has not been processed yet";
                return;
            }
        }
        else
        {
            // This plus is not connected, use the user-defined value
            inputs.insert(plugName, node->getProperties()[plugName]);
        }
    }
}

quint16 ComposerScheduler::maxThreads() const
{
    if(_settings.useMultiThreading)
    {
        if(_settings.useOptimalThreadsCount)
        {
            return QThread::idealThreadCount();
        }
        else
        {
            return _settings.fixedThreadsCount;
        }
    }
    else
    {
        return 1;
    }
}

void ComposerScheduler::processNexts()
{
    while(_currentExecutors.count() + _oldExecutors.count() < maxThreads())
    {
        bool nodeAddedForProcessing = false;

        QList<const Node *> potentialNodes = _model->getNodes();
        for(const Node *node : _processedNodes.keys())
        {
            // Remove nodes which have already been processed
            potentialNodes.removeAll(node);
        }
        for(ComposerExecutor *executor : _currentExecutors)
        {
            // Remove nodes which are being processed
            potentialNodes.removeAll(executor->getNode());
        }

        for(const Node *node : potentialNodes)
        {
            if(allInputsProcessed(node))
            {
                // All inputs of node have been processed, we can process it now !
                Properties inputs;
                makeInputs(node, inputs);

                ComposerExecutor *executor = new ComposerExecutor(this);
                connect(executor, SIGNAL(nodeProcessed()), executor, SLOT(deleteLater()));
                connect(executor, SIGNAL(nodeProcessed()), SLOT(onNodeProcessed()));
                connect(executor, SIGNAL(executionProgress(qreal)), SLOT(onProgress(qreal)));
                executor->process(node, inputs);

                nodeAddedForProcessing = true;

                _currentExecutors << executor;

                emit executorStarted(node);

                break;
            }
        }

        if(!nodeAddedForProcessing)
        {
            // There is no other node we can process yet
            if(_currentExecutors.count() == 0)
            {
                // And no executor is running => we are globally over
                foreach(const Node *node, _keepProcessingNodes)
                {
                    reProcessFromNode(node);
                }
                _keepProcessingNodes.clear();
            }

            return;
        }
    }
}

void ComposerScheduler::reProcessFromNode(const Node *node)
{
    for(const Node *descendantNode : _model->findDescendantNodes(node))
    {
        cancelExecutors(descendantNode);

        _processedNodes.remove(descendantNode);
    }

    processNexts();
}

void ComposerScheduler::reProcessAll()
{
    for(ComposerExecutor *executor : _currentExecutors)
    {
        cancelExecutor(executor);
    }

    _processedNodes.clear();

    processNexts();
}

void ComposerScheduler::invalidateFromNode(const Node *node, bool includeCurrent)
{
    for(const Node *descendantNode : _model->findDescendantNodes(node, includeCurrent))
    {
        cancelExecutors(descendantNode);

        // Indicate that node has been processed, but with no result
       _processedNodes[descendantNode] = Properties();

       emit nodeInvalid(descendantNode);
    }
}

void ComposerScheduler::cancelExecutors(const Node *node)
{
    for(ComposerExecutor *executor : _currentExecutors)
    {
        if(executor->getNode() == node)
        {
            cancelExecutor(executor);
        }
    }
}

void ComposerScheduler::cancelExecutor(ComposerExecutor *executor)
{
    // Register executor as old
    if(_currentExecutors.removeAll(executor) > 0)
    {
        _oldExecutors << executor;

        // Disconnect progress signal we may keep receiving
        disconnect(executor, SIGNAL(executionProgress(qreal)), this, SLOT(onProgress(qreal)));

        // Signal that the executor is aborted
        emit executorEnded(executor->getNode(), Properties(), Properties(), -1, "");
    }
    else
    {
        qCritical() << "Given executor is not registered as current";
    }
}

void ComposerScheduler::clearUnusedCache()
{
    for(auto iterator = _processedNodes.begin() ; iterator != _processedNodes.end() ; ++iterator)
    {
        const Node *node = iterator.key();
        Properties &outputs = iterator.value();

        for(const Plug *output : node->getOutputs())
        {
            bool allDescendantNodesProcessed = true;

            for(const Node *descendantNode : _model->findDirectDescendantNodes(output))
            {
                if(!_processedNodes.contains(descendantNode))
                {
                    allDescendantNodesProcessed = false;
                    break;
                }
            }

            if(allDescendantNodesProcessed)
            {
                // The data is no more useful, remove it right now to save memory
                outputs.remove(output->getDefinition().name);
            }
        }
    }
}

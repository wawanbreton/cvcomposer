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

#include "composerexecutor.h"

#include <QDebug>

#include "nodes/genericnode.h"
#include "processors/abstractprocessor.h"


ComposerExecutor::ComposerExecutor(QObject *parent) :
    QThread(parent),
    _job()
{
    connect(this, SIGNAL(finished()), SLOT(onFinished()));
}

void ComposerExecutor::processNode(GenericNode *node, const QList<cv::Mat> &inputs)
{
    if(not isRunning())
    {
        _job.node = node;
        _job.processor = createProcessor(node);
        _job.inputs = inputs;
        _job.success = false;

        start();
    }
    else
    {
        qCritical() << "ComposerExecutor::processNode"
                    << "Unable to process node while already running";
    }
}

void ComposerExecutor::run()
{
    try
    {
        _job.outputs = _job.processor->process(_job.inputs);
        _job.success = true;
    }
    catch(const std::exception &exception)
    {
        qDebug() << "Exception when executing node" << _job.node->getUserReadableName() << " : " << exception.what();
    }
}

void ComposerExecutor::onFinished()
{
    if(_job.success)
    {
        _job.node->signalProcessDone(_job.outputs, _job.inputs);
    }
    else
    {
        _job.node->signalProcessUnavailable();
    }
    emit nodeProcessed(_job.success, _job.outputs);
}

AbstractProcessor *ComposerExecutor::createProcessor(GenericNode *node)
{
    QMetaType processorType(QMetaType::type((node->getName() + "Processor").toUtf8()));
    if(processorType.isValid())
    {
        AbstractProcessor *processor = static_cast<AbstractProcessor *>(processorType.create());
        processor->setProperties(node->getProperties());
        return processor;
    }
    else
    {
        qCritical() << "ComposerScheduler::createProcessor"
                    << "Unable to find processor for node" << node->getName();
        return NULL;
    }
}

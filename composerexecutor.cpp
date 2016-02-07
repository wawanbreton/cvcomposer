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

#include "genericnode.h"
#include "processors/abstractprocessor.h"


ComposerExecutor::ComposerExecutor(QObject *parent) :
    QThread(parent),
    _node(NULL),
    _processor(NULL),
    _inputs(),
    _outputs(),
    _success(false)
{
    connect(this, SIGNAL(finished()), SLOT(onFinished()));
}

void ComposerExecutor::process(GenericNode *node, const Properties &inputs)
{
    _success = false;
    _node = node;
    _processor = createProcessor(node);
    _inputs = inputs;
    start();
}

void ComposerExecutor::run()
{
    try
    {
        _outputs = _processor->process(_inputs);
        _success = true;
    }
    catch(const std::exception &exception)
    {
        qDebug() << "Exception when executing node" << _node->getUserReadableName() << " : " << exception.what();
    }
}

void ComposerExecutor::onFinished()
{
    // Make local copies in case local values are modified during the signal emission
    Properties inputs = _inputs;
    Properties outputs = _outputs;
    bool success = _success;

    delete _processor;

    if(success)
    {
        _node->signalProcessDone(outputs, inputs);
    }
    else
    {
        _node->signalProcessUnavailable();
    }
    emit nodeProcessed(success, outputs);
}

AbstractProcessor *ComposerExecutor::createProcessor(GenericNode *node)
{
    QMetaType processorType(QMetaType::type((node->getName() + "Processor").toUtf8()));
    if(processorType.isValid())
    {
        return static_cast<AbstractProcessor *>(processorType.create());
    }
    else
    {
        qCritical() << "ComposerScheduler::createProcessor"
                    << "Unable to find processor for node" << node->getName();
        return NULL;
    }
}

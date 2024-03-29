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
#include <QElapsedTimer>
#include <QTimer>

#include "model/node.h"
#include "processor/abstractprocessor.h"


ComposerExecutor::ComposerExecutor(const Node *node,
                                   const Properties &inputs,
                                   const QSharedPointer<AbstractProcessor> processor,
                                   QObject *parent) :
    QThread(parent),
    _node(node),
    _processor(processor),
    _inputs(inputs),
    _outputs(),
    _error()
{
    connect(this, &ComposerExecutor::finished, this, &ComposerExecutor::onFinished);
}

void ComposerExecutor::process()
{
    connect(_processor.data(), &AbstractProcessor::progress,
            this,              &ComposerExecutor::executionProgress);

    qDebug() << "start   " << this << _node->getUserReadableName();

    start();
}

void ComposerExecutor::run()
{
    QElapsedTimer timer;
    timer.start();

    try
    {
        _outputs = _processor->process(_inputs);
    }
    catch(const std::exception &exception)
    {
        qDebug() << "Exception when executing node"
                 << _node->getUserReadableName() << " : " << exception.what();
        _error = exception.what();
    }

    _duration = timer.elapsed();
}

void ComposerExecutor::onFinished()
{
    qDebug() << "finished" << this << _error;

    // Send the signal when we are fully finished
    emit nodeProcessed();
}

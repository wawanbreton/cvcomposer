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

#include "nodes/abstractnode.h"


ComposerExecutor::ComposerExecutor(QObject *parent) :
    QThread(parent),
    _job()
{
    connect(this, SIGNAL(finished()), SLOT(onFinished()));
}

void ComposerExecutor::processNode(AbstractNode *node, const QList<cv::Mat> &inputs)
{
    if(not isRunning())
    {
        _job.node = node;
        _job.inputs = inputs;

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
    _job.outputs = _job.node->process(_job.inputs);
}

void ComposerExecutor::onFinished()
{
    _job.node->signalProcessDone(_job.outputs, _job.inputs);
    emit nodeProcessed(_job.outputs);
}

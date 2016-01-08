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

#include "abstractnode.h"

#include <QDebug>


AbstractNode::AbstractNode(quint8 nbInputs, quint8 nbOutputs, const QString &userReadableName, QObject *parent) :
    QObject(parent),
    _inputs(),
    _outputs(),
    _userReadableName(userReadableName)
{
    _inputs.reserve(nbInputs);
    for(quint8 i = 0 ; i < nbInputs ; i++)
    {
        _inputs << QUuid::createUuid();
    }

    for(quint8 i = 0 ; i < nbOutputs ; i++)
    {
        _outputs << QUuid::createUuid();
    }
}

quint8 AbstractNode::getNbInputs() const
{
    return _inputs.count();
}

const QList<QUuid> &AbstractNode::getInputs() const
{
    return _inputs;
}

bool AbstractNode::hasInput(const QUuid &input) const
{
    return _inputs.contains(input);
}

quint8 AbstractNode::getNbOutputs() const
{
    return _outputs.count();
}

const QList<QUuid> &AbstractNode::getOutputs() const
{
    return _outputs;
}

bool AbstractNode::hasOutput(const QUuid &output) const
{
    return _outputs.contains(output);
}

const QString &AbstractNode::getUserReadableName() const
{
    return _userReadableName;
}

QList<cv::Mat> AbstractNode::process(const QList<cv::Mat> &inputs)
{
    Q_ASSERT(inputs.count() == getNbInputs());
    QList<cv::Mat> outputs = processImpl(inputs);
    Q_ASSERT(outputs.count() == getNbOutputs());

    return outputs;
}

void AbstractNode::signalProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs)
{
    emit processDone(outputs, inputs);
}

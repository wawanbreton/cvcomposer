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


AbstractNode::AbstractNode(quint8 nbInputs, quint8 nbOutputs, const QString &userReadableName, QObject *parent) :
    QObject(parent),
    _nbInputs(nbInputs),
    _nbOutputs(nbOutputs),
    _userReadableName(userReadableName)
{
}

quint8 AbstractNode::getNbInputs() const
{
    return _nbInputs;
}

quint8 AbstractNode::getNbOutputs() const
{
    return _nbOutputs;
}

const QString &AbstractNode::getUserReadableName() const
{
    return _userReadableName;
}

QList<cv::Mat> AbstractNode::process(const QList<cv::Mat> &inputs)
{
    Q_ASSERT(inputs.count() == _nbInputs);
    QList<cv::Mat> outputs = processImpl(inputs);
    Q_ASSERT(outputs.count() == _nbOutputs);

    emit processDone(outputs, inputs);
    return outputs;
}

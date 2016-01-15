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

#include "blurnode.h"

#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>


BlurNode::BlurNode(QObject *parent) :
    AbstractNode(1, 1, tr("Blur"), parent),
    _size(1, 1),
    _anchor(-1, -1)
{
}

QList<cv::Mat> BlurNode::processImpl(const QList<cv::Mat> &inputs)
{
    cv::Mat blurred = inputs[0].clone();
    cv::blur(inputs[0], blurred, _size, _anchor);
    return QList<cv::Mat>() << blurred;
}

const cv::Size BlurNode::getSize() const
{
    return _size;
}

void BlurNode::setSize(const cv::Size &size)
{
    if(size != _size)
    {
        _size = size;
        emit changed();
    }
}

const cv::Point &BlurNode::getAnchor() const
{
    return _anchor;
}

void BlurNode::setAnchor(const cv::Point &anchor)
{
    if(anchor != _anchor)
    {
        _anchor = anchor;
        emit changed();
    }
}

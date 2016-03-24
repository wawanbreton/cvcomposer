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

#include "rectangleprocessor.h"

#include "global/cvutils.h"


RectangleProcessor::RectangleProcessor()
{
    addInput("position", PlugType::Point, QVariant::fromValue(cv::Point(0, 0)));
    addInput("size", PlugType::Size, QVariant::fromValue(cv::Size(10, 10)));

    addOutput("rectangle", PlugType::Rectangle);
}

Properties RectangleProcessor::processImpl(const Properties &inputs)
{
    cv::Point position = inputs["position"].value<cv::Point>();
    cv::Size size = inputs["size"].value<cv::Size>();

    cv::Rect rectangle(position, size);

    Properties outputs;
    outputs.insert("rectangle", QVariant::fromValue(rectangle));
    return outputs;
}

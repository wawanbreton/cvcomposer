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

#include "ellipseprocessor.h"

#include "global/cvutils.h"
#include "global/cvconstants.h"
#include "model/ellipse.h"


EllipseProcessor::EllipseProcessor()
{
    addInput("center", PlugType::Point);
    addInput("axes",
             PlugType::Size,
             QVariant::fromValue(cv::Size(CvConstants::defaultShapeSide,
                                          CvConstants::defaultShapeSide)));

    Properties anglesProperties;
    anglesProperties.insert("maximum", 360);
    addInput("angle", PlugType::Double, 0, anglesProperties);
    addInput("start angle", PlugType::Double, 0, anglesProperties);
    addInput("end angle", PlugType::Double, 360, anglesProperties);

    addOutput("ellipse", PlugType::Ellipse);
}

Properties EllipseProcessor::processImpl(const Properties &inputs)
{
    Ellipse ellipse;
    ellipse.center = inputs["center"].value<cv::Point>();
    ellipse.axes = inputs["axes"].value<cv::Size>();
    ellipse.angle = inputs["angle"].toDouble();
    ellipse.startAngle = inputs["start angle"].toDouble();
    ellipse.endAngle = inputs["end angle"].toDouble();

    Properties outputs;
    outputs.insert("ellipse", QVariant::fromValue(ellipse));
    return outputs;
}


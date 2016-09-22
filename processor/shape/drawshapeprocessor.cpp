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

#include "drawshapeprocessor.h"

#include "global/cvutils.h"
#include "model/circle.h"


DrawShapeProcessor::DrawShapeProcessor()
{
    addInput("input image", PlugType::Image);
    addInput("shape", PlugType::Generic, QVariant(), Properties(), ThreeStateBool::None, true);
    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(255, 255, 255, 255)));

    Properties thicknessProperties;
    thicknessProperties.insert("minimum", -1);
    thicknessProperties.insert("decimals", 0);
    addInput("thickness", PlugType::Double, 1, thicknessProperties);

    addEnumerationInput("line type", CvUtils::makeLineTypeValues(), 8);

    Properties shiftProperties;
    shiftProperties.insert("decimals", 0);
    addInput("shift", PlugType::Double, 0, shiftProperties);

    addOutput("output image", PlugType::Image);
}

Properties DrawShapeProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    QList<QVariant> shapes = inputs["shape"].value<QList<QVariant>>();
    cv::Scalar color = inputs["color"].value<cv::Scalar>();
    int thickness = inputs["thickness"].toInt();
    int lineType = inputs["line type"].toInt();
    int shift = inputs["shift"].toInt();

    cv::Mat outputImage = inputImage.clone();

    foreach(const QVariant &shape, shapes)
    {
        if(shape.userType() == qMetaTypeId<Circle>())
        {
            Circle circle = shape.value<Circle>();
            cv::circle(outputImage, circle.center, circle.radius, color, thickness, lineType, shift);
        }
        else if(shape.userType() == qMetaTypeId<cv::Rect>())
        {
            cv::Rect rect = shape.value<cv::Rect>();
            cv::rectangle(outputImage, rect, color, thickness, lineType, shift);
        }
    }

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


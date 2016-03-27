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

#include "drawrectangleprocessor.h"

#include "global/cvutils.h"

#include <opencv2/core/core_c.h>



DrawRectangleProcessor::DrawRectangleProcessor()
{
    addInput("input image", PlugType::Image);
    addInput("rectangle", PlugType::Rectangle);
    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(255, 255, 255, 255)));

    Properties thicknessProperties;
    thicknessProperties.insert("minimum", -1);
    thicknessProperties.insert("decimals", 0);
    addInput("thickness", PlugType::Double, 1, thicknessProperties);

    QList<QPair<QString, QVariant> > lineTypeValues;
    lineTypeValues << QPair<QString, QVariant>("8-connected", 8);
    lineTypeValues << QPair<QString, QVariant>("4-connected", 4);
    lineTypeValues << QPair<QString, QVariant>("Antialiased", CV_AA);
    addEnumerationInput("line type", lineTypeValues, 8);

    Properties shiftProperties;
    shiftProperties.insert("decimals", 0);
    addInput("shift", PlugType::Double, 0, shiftProperties);

    addOutput("output image", PlugType::Image);
}

Properties DrawRectangleProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();

    cv::Mat outputImage = inputImage.clone();
    cv::rectangle(outputImage,
                  inputs["rectangle"].value<cv::Rect>(),
                  inputs["color"].value<cv::Scalar>(),
                  inputs["thickness"].toInt(),
                  inputs["line type"].toInt(),
                  inputs["shift"].toInt());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}

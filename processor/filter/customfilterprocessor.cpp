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

#include "customfilterprocessor.h"

#include "global/cvutils.h"

#include <opencv2/imgproc/imgproc.hpp>


CustomFilterProcessor::CustomFilterProcessor()
{
    addInput("input image", PlugType::Image);

    addInput("kernel", PlugType::Kernel);

    Properties anchorProperties;
    anchorProperties.insert("x-minimum", -1);
    anchorProperties.insert("y-minimum", -1);
    addInput("anchor", PlugType::Point, QVariant::fromValue(cv::Point(-1, -1)), anchorProperties);

    Properties deltaProperties;
    deltaProperties.insert("minimum", -256.0);
    deltaProperties.insert("maximum", 256.0);
    addInput("delta", PlugType::Double, 0.0, deltaProperties);

    QList<QPair<QString, QVariant> > depthValues;
    depthValues << QPair<QString, QVariant>("Same", -1);
    depthValues << QPair<QString, QVariant>("16S", CV_16S);
    depthValues << QPair<QString, QVariant>("32F", CV_32F);
    depthValues << QPair<QString, QVariant>("64F", CV_64F);
    addEnumerationInput("depth", depthValues, -1);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addOutput("output image", PlugType::Image);
}

Properties CustomFilterProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage;
    cv::filter2D(inputImage,
                 outputImage,
                 inputs["depth"].toInt(),
                 inputs["kernel"].value<cv::Mat>(),
                 inputs["anchor"].value<cv::Point>(),
                 inputs["delta"].toDouble(),
                 inputs["border"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(outputImage));
    return properties;
}

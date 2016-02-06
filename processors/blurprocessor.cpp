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

#include "blurprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "cvutils.h"


BlurProcessor::BlurProcessor() :
    AbstractProcessor()
{
    addInput("input image", PlugType::Image);

    Properties sizeProperties;
    sizeProperties.insert("width-minimum", 1);
    sizeProperties.insert("width-singleStep", 2);
    sizeProperties.insert("height-minimum", 1);
    sizeProperties.insert("height-singleStep", 2);
    addInput("size", PlugType::Size,  QVariant::fromValue(cv::Size(1, 1)), sizeProperties);

    Properties anchorProperties;
    anchorProperties.insert("x-minimum", -1);
    anchorProperties.insert("y-minimum", -1);
    addInput("anchor", PlugType::Point, QVariant::fromValue(cv::Point(-1, -1)), anchorProperties);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addOutput("output image", PlugType::Image);
}

Properties BlurProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat blurred = inputImage.clone();
    cv::blur(inputImage,
             blurred,
             inputs["size"].value<cv::Size>(),
             inputs["anchor"].value<cv::Point>(),
             inputs["border"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(blurred));
    return properties;
}


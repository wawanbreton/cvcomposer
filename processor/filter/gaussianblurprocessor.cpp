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

#include "gaussianblurprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"


GaussianBlurProcessor::GaussianBlurProcessor() :
    AbstractProcessor()
{
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    Properties sizeProperties;
    sizeProperties.insert("width-minimum", 1);
    sizeProperties.insert("width-singleStep", 2);
    sizeProperties.insert("height-minimum", 1);
    sizeProperties.insert("height-singleStep", 2);
    addInput("size", PlugType::Size,  QVariant::fromValue(cv::Size(1, 1)), sizeProperties);

    addInput("sigma X", PlugType::Double, 0.0);
    addInput("sigma Y", PlugType::Double, 0.0);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addOutput("output image", PlugType::Image, ProcessorListType::Simple);
}

Properties GaussianBlurProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat blurred;
    cv::GaussianBlur(inputImage,
                     blurred,
                     inputs["size"].value<cv::Size>(),
                     inputs["sigma X"].toDouble(),
                     inputs["sigma Y"].toDouble(),
                     inputs["border"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(blurred));
    return properties;
}


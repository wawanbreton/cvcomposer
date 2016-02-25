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

#include "bilateralfilterprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"


BilateralFilterProcessor::BilateralFilterProcessor()
{
    addInput("input image", PlugType::Image);

    Properties diameterProperties;
    diameterProperties.insert("singleStep", 2);
    diameterProperties.insert("decimals", 0);
    addInput("diameter", PlugType::Double, 1.0, diameterProperties);

    addInput("sigma color", PlugType::Double, 2.0);
    addInput("sigma space", PlugType::Double, 0.5);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addOutput("output image", PlugType::Image);
}

Properties BilateralFilterProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat blurred;
    cv::bilateralFilter(inputImage,
                        blurred,
                        inputs["diameter"].toInt(),
                        inputs["sigma color"].toDouble(),
                        inputs["sigma space"].toDouble(),
                        inputs["border"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(blurred));
    return properties;
}

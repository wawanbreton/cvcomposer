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

#include "thresholdprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "cvutils.h"


ThresholdProcessor::ThresholdProcessor()
{
    addInput("input image", PlugType::Image);

    QList<QPair<QString, QVariant> > types;
    types << QPair<QString, QVariant>("Binary",           cv::THRESH_BINARY);
    types << QPair<QString, QVariant>("Binary inverted",  cv::THRESH_BINARY_INV);
    types << QPair<QString, QVariant>("Truncate",         cv::THRESH_TRUNC);
    types << QPair<QString, QVariant>("To zero",          cv::THRESH_TOZERO);
    types << QPair<QString, QVariant>("To zero inverted", cv::THRESH_TOZERO_INV);
    addEnumerationInput("type", types, cv::THRESH_BINARY);

    addInput("threshold",     PlugType::Double, 128.0);
    addInput("maximum value", PlugType::Double, 256.0);

    addOutput("output image", PlugType::Image);
}

Properties ThresholdProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage;

    cv::threshold(inputImage,
                  outputImage,
                  inputs["threshold"].toDouble(),
                  inputs["maximum value"].toDouble(),
                  inputs["type"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(outputImage));
    return properties;
}

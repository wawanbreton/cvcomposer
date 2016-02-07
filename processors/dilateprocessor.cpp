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

#include "dilateprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "cvutils.h"


DilateProcessor::DilateProcessor()
{
    addInput("input image", PlugType::Image);

    QList<QPair<QString, QVariant> > types;
    types << QPair<QString, QVariant>("Rectangle", cv::MORPH_RECT);
    types << QPair<QString, QVariant>("Cross",     cv::MORPH_CROSS);
    types << QPair<QString, QVariant>("Ellipse",   cv::MORPH_ELLIPSE);
    addEnumerationInput("type", types, cv::MORPH_RECT);

    Properties sizeProperties;
    sizeProperties.insert("minimum", 1);
    sizeProperties.insert("decimals", 0);
    addInput("size", PlugType::Double, 1, sizeProperties);

    Properties anchorProperties;
    anchorProperties.insert("x-minimum", -1);
    anchorProperties.insert("y-minimum", -1);
    addInput("anchor", PlugType::Point, QVariant::fromValue(cv::Point(-1, -1)), anchorProperties);

    Properties iterationProperties;
    iterationProperties.insert("minimum", 1);
    iterationProperties.insert("decimals", 0);
    addInput("iterations", PlugType::Double, 1.0, iterationProperties);

    addEnumerationInput("border type", CvUtils::makeBlurBorderValues(), cv::BORDER_CONSTANT);

    addOutput("output image", PlugType::Image);
}

Properties DilateProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    int size = inputs["size"].toInt();
    cv::Mat dilated;

    cv::Mat element = cv::getStructuringElement(inputs["type"].toInt(),
                                                cv::Size(2 * size + 1, 2 * size+1),
                                                cv::Point(size, size));
    cv::dilate(inputImage,
               dilated,
               element,
               inputs["anchor"].value<cv::Point>(),
               inputs["iterations"].toInt(),
               inputs["border type"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(dilated));
    return properties;
}

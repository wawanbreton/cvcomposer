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

#include "morphologytransformationprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


MorphologyTransformationProcessor::MorphologyTransformationProcessor()
{
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    QList<QPair<QString, QVariant> > operators;
    operators << QPair<QString, QVariant>("Erode",     cv::MORPH_ERODE);
    operators << QPair<QString, QVariant>("Dilate",    cv::MORPH_DILATE);
    operators << QPair<QString, QVariant>("Open",      cv::MORPH_OPEN);
    operators << QPair<QString, QVariant>("Close",     cv::MORPH_CLOSE);
    operators << QPair<QString, QVariant>("Gradient",  cv::MORPH_GRADIENT);
    operators << QPair<QString, QVariant>("Top hat",   cv::MORPH_TOPHAT);
    operators << QPair<QString, QVariant>("Black hat", cv::MORPH_BLACKHAT);
    addEnumerationInput("operator", operators, cv::MORPH_ERODE);

    QList<QPair<QString, QVariant> > types;
    types << QPair<QString, QVariant>("Rectangle", cv::MORPH_RECT);
    types << QPair<QString, QVariant>("Cross",     cv::MORPH_CROSS);
    types << QPair<QString, QVariant>("Ellipse",   cv::MORPH_ELLIPSE);
    addEnumerationInput("shape", types, cv::MORPH_RECT);

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

    addInput("border color",
             PlugType::Color,
             QVariant::fromValue(cv::morphologyDefaultBorderValue()));

    addOutput("output image", PlugType::Image, ProcessorListType::Simple);
}

Properties MorphologyTransformationProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    int size = inputs["size"].toInt();
    cv::Mat outputImage;

    cv::Mat element = cv::getStructuringElement(inputs["shape"].toInt(),
                                                cv::Size(2 * size + 1, 2 * size+1),
                                                cv::Point(size, size));
    cv::morphologyEx(inputImage,
                     outputImage,
                     inputs["operator"].toInt(),
                     element,
                     inputs["anchor"].value<cv::Point>(),
                     inputs["iterations"].toInt(),
                     inputs["border type"].toInt(),
                     inputs["border color"].value<cv::Scalar>());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(outputImage));
    return properties;
}

// Copyright 2021 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "adaptativethresholdprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


AdaptativeThresholdProcessor::AdaptativeThresholdProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    addInput("maximum value", PlugType::Double, 256.0);

    QList<QPair<QString, QVariant>> methods;
    methods << QPair<QString, QVariant>("Mean",      cv::ADAPTIVE_THRESH_MEAN_C);
    methods << QPair<QString, QVariant>("Gaussian",  cv::ADAPTIVE_THRESH_GAUSSIAN_C);
    addEnumerationInput("method", methods);

    QList<QPair<QString, QVariant>> types;
    types << QPair<QString, QVariant>("Binary",           cv::THRESH_BINARY);
    types << QPair<QString, QVariant>("Binary inverted",  cv::THRESH_BINARY_INV);
    addEnumerationInput("type", types);

    Properties blockSizeProperties;
    blockSizeProperties.insert("decimals", 0);
    blockSizeProperties.insert("minimum", 3);
    addInput("block size", PlugType::Double, 3.0, blockSizeProperties);

    addInput("constant", PlugType::Double, 0.0);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    //Help
    addHelpMessage("adaptiveThreshold",
                   CvUtils::makeUrl({"d7", "d1b", "group__imgproc__misc"}, "ga72b913f352e4a1b1b397736707afcde3"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d7", "d4d", "tutorial_py_thresholding"}),
                   HelpMessageType::Tutorial);
}

Properties AdaptativeThresholdProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage;

    cv::adaptiveThreshold(inputImage,
                          outputImage,
                          inputs["maximum value"].toDouble(),
                          inputs["method"].toInt(),
                          inputs["type"].toInt(),
                          inputs["block size"].toInt(),
                          inputs["constant"].toReal());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(outputImage));
    return properties;
}


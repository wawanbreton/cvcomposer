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

#include "medianblurprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


MedianBlurProcessor::MedianBlurProcessor() :
    AbstractProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    Properties sizeProperties;
    sizeProperties.insert("singleStep", 2);
    sizeProperties.insert("decimals", 0);
    addInput("size", PlugType::Double,  1.0, sizeProperties);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("medianBlur",
                   CvUtils::makeUrl({"d4", "d86", "group__imgproc__filter"}, "ga564869aa33e58769b4469101aac458f9"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d4", "d13", "tutorial_py_filtering"}),
                   HelpMessageType::Tutorial);
}

Properties MedianBlurProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat blurred;
    cv::medianBlur(inputImage, blurred, inputs["size"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(blurred));
    return properties;
}

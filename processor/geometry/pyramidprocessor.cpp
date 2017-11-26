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

#include "pyramidprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


PyramidProcessor::PyramidProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    QList<QPair<QString, QVariant>> typeValues;
    typeValues << QPair<QString, QVariant>("Up", "up");
    typeValues << QPair<QString, QVariant>("Down", "down");
    addEnumerationInput("type", typeValues, "down");

    Properties iterationProperties;
    iterationProperties.insert("decimals", 0);
    iterationProperties.insert("minimum", 1);
    addInput("iterations", PlugType::Double, 1, iterationProperties);

    Properties sizeProperties;
    sizeProperties.insert("width-minimum", 0);
    sizeProperties.insert("width-singleStep", 1);
    sizeProperties.insert("height-minimum", 0);
    sizeProperties.insert("height-singleStep", 1);
    addInput("size", PlugType::Size, QVariant::fromValue(cv::Size(0, 0)), sizeProperties);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("pyrUp", CvUtils::makeUrl({"d4", "d86", "group__imgproc__filter"}, "gada75b59bdaaca411ed6fee10085eb784"));
    addHelpMessage("pyrDown", CvUtils::makeUrl({"d4", "d86", "group__imgproc__filter"}, "gaf9bba239dfca11654cb7f50f889fc2ff"));
    addHelpMessage("tutorial", CvUtils::makeUrl({"d4", "d1f", "tutorial_pyramids"}));
}

Properties PyramidProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage = inputImage.clone();
    QString method = inputs["type"].toString();
    int iterations = inputs["iterations"].toInt();
    cv::Size size = inputs["size"].value<cv::Size>();
    if(size.width <= 0 || size.height <= 0)
    {
        size = cv::Size();
    }
    int border = inputs["border"].toInt();

    auto function = method == "up" ? cv::pyrUp : cv::pyrDown;
    for(int i = 0 ; i < iterations ; i++)
    {
        function(outputImage, outputImage, size, border);
    }

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}

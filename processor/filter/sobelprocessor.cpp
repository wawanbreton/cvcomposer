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

#include "sobelprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


SobelProcessor::SobelProcessor()
{
    addInput("input image", PlugType::Image);

    QList<QPair<QString, QVariant> > methods;
    methods << QPair<QString, QVariant>("Sobel", 0);
    methods << QPair<QString, QVariant>("Scharr", 1);
    addEnumerationInput("method", methods, 0);

    Properties depthProperties;
    depthProperties.insert("decimals", 0);
    depthProperties.insert("minimum", -1);
    addInput("depth", PlugType::Double, -1, depthProperties);

    addInput("derivates", PlugType::Size, QVariant::fromValue(cv::Size(1, 1)));

    Properties kernelSizeProperties;
    kernelSizeProperties.insert("decimals", 0);
    kernelSizeProperties.insert("minimum", 1);
    kernelSizeProperties.insert("singleStep", 2);
    addInput("kernel size", PlugType::Double, 3, kernelSizeProperties);

    addInput("scale", PlugType::Double, 1);
    addInput("delta", PlugType::Double, 0);

    addEnumerationInput("border", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addOutput("output image", PlugType::Image);
}

Properties SobelProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    int depth =  inputs["depth"].toInt();
    cv::Size derivates = inputs["derivates"].value<cv::Size>();
    double scale = inputs["scale"].toDouble();
    double delta = inputs["delta"].toDouble();
    int border = inputs["border"].toInt();
    cv::Mat outputImage;

    if(inputs["method"].toInt() == 0)
    {
        cv::Sobel(inputImage,
                  outputImage,
                  depth,
                  derivates.width,
                  derivates.height,
                  inputs["kernel size"].toInt(),
                  scale,
                  delta,
                  border);
    }
    else
    {
        cv::Scharr(inputImage,
                   outputImage,
                   depth,
                   derivates.width,
                   derivates.height,
                   scale,
                   delta,
                   border);
    }

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


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

#include "makeborderprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


MakeBorderProcessor::MakeBorderProcessor()
{
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    Properties marginsProperties;
    marginsProperties.insert("decimals", 0);
    addInput("top",    PlugType::Double, 5, marginsProperties);
    addInput("bottom", PlugType::Double, 5, marginsProperties);
    addInput("left",   PlugType::Double, 5, marginsProperties);
    addInput("right",  PlugType::Double, 5, marginsProperties);

    addEnumerationInput("type", CvUtils::makeBlurBorderValues(), cv::BORDER_DEFAULT);

    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(0)));

    addOutput("output image", PlugType::Image, ProcessorListType::Simple);
}

Properties MakeBorderProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;

    cv::copyMakeBorder(inputs["input image"].value<cv::Mat>(),
                       outputImage,
                       inputs["top"].toInt(),
                       inputs["bottom"].toInt(),
                       inputs["left"].toInt(),
                       inputs["right"].toInt(),
                       inputs["type"].toInt(),
                       inputs["color"].value<cv::Scalar>());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


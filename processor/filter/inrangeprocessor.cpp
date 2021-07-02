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

#include "inrangeprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"


InRangeProcessor::InRangeProcessor()
{
    // Inputs
    addInput("input image",  PlugType::Image);

    Properties boundariesProperties;
    boundariesProperties.insert("decimals", 0);

    addInput("lower boundary 0", PlugType::Double, 0.0, boundariesProperties);
    addInput("lower boundary 1", PlugType::Double, 0.0, boundariesProperties);
    addInput("lower boundary 2", PlugType::Double, 0.0, boundariesProperties);
    addInput("upper boundary 0", PlugType::Double, 360.0, boundariesProperties);
    addInput("upper boundary 1", PlugType::Double, 255.0, boundariesProperties);
    addInput("upper boundary 2", PlugType::Double, 255.0, boundariesProperties);

    // Outputs
    addOutput("output image", PlugType::Image);

    // Help
    addHelpMessage("inRange",
                   CvUtils::makeUrl({"d2", "de8", "group__core__array"}, "ga48af0ab51e36436c5d04340e036ce981"),
                   HelpMessageType::Function);
}

Properties InRangeProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;

    cv::inRange(inputs["input image"].value<cv::Mat>(),
                cv::Scalar(inputs["lower boundary 0"].value<double>(),
                           inputs["lower boundary 1"].value<double>(),
                           inputs["lower boundary 2"].value<double>()),
                cv::Scalar(inputs["upper boundary 0"].value<double>(),
                           inputs["upper boundary 1"].value<double>(),
                           inputs["upper boundary 2"].value<double>()),
                outputImage);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


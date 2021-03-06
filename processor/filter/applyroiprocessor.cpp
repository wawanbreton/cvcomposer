// Copyright 2019 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "applyroiprocessor.h"

#include "global/cvutils.h"


ApplyRoiProcessor::ApplyRoiProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);
    addInput("roi", PlugType::Image);

    // Outputs
    addOutput("output image", PlugType::Image);

    // Help
    addHelpMessage("Mat::copyTo",
                   CvUtils::makeUrl({"d3", "d63", "classcv_1_1Mat"}, "ab931fe25e359c1fe727e526aa5693ea9"),
                   HelpMessageType::Function);
}

Properties ApplyRoiProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat roi = inputs["roi"].value<cv::Mat>();
    cv::Mat outputImage;

    inputImage.copyTo(outputImage, roi);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


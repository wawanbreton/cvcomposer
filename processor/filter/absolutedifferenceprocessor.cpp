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

#include "absolutedifferenceprocessor.h"

#include "global/cvutils.h"


AbsoluteDifferenceProcessor::AbsoluteDifferenceProcessor()
{
    // Inputs
    addInput("input image 1", PlugType::Image);
    addInput("input image 2", PlugType::Image);

    // Outputs
    addOutput("output image", PlugType::Image);

    // Help
    addHelpMessage("absdiff",
                   CvUtils::makeUrl({"d2", "de8", "group__core__array"}, "ga6fef31bc8c4071cbc114a758a2b79c14"),
                   HelpMessageType::Function);
}

Properties AbsoluteDifferenceProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;
    cv::absdiff(inputs["input image 1"].value<cv::Mat>(),
                inputs["input image 2"].value<cv::Mat>(),
                outputImage);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


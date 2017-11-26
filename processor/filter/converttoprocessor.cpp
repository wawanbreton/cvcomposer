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

#include "converttoprocessor.h"

#include "global/cvconstants.h"
#include "global/cvutils.h"


ConvertToProcessor::ConvertToProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    Properties alphaProperties;
    alphaProperties.insert("singleStep", 0.1);
    addInput("alpha", PlugType::Double, 1.0, alphaProperties);

    Properties betaProperties;
    betaProperties.insert("minimum", -CvConstants::defaultDoubleMax);
    addInput("beta", PlugType::Double, 0.0, betaProperties);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("convertTo",
                   CvUtils::makeUrl({"d3", "d63", "classcv_1_1Mat"}, "a3f356665bb0ca452e7d7723ccac9a810"));
}

Properties ConvertToProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage;
    inputImage.convertTo(outputImage,
                         -1,
                         inputs["alpha"].toDouble(),
                         inputs["beta"].toDouble());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}

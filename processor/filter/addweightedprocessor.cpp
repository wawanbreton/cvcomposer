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

#include "addweightedprocessor.h"

#include "global/cvconstants.h"
#include "global/cvutils.h"


AddWeightedProcessor::AddWeightedProcessor()
{
    // Inputs
    Properties coefficientsProperties;
    coefficientsProperties.insert("minimum", -CvConstants::defaultDoubleMax);
    coefficientsProperties.insert("singleStep", 0.1);

    addInput("input image 1", PlugType::Image);
    addInput("alpha", PlugType::Double, 0.5, coefficientsProperties);
    addInput("input image 2", PlugType::Image);
    addInput("beta", PlugType::Double, 0.5, coefficientsProperties);
    addInput("gamma", PlugType::Double, 0.0, coefficientsProperties);

    Properties depthProperties;
    depthProperties.insert("decimals", 0);
    depthProperties.insert("minimum", -1);
    addInput("depth", PlugType::Double, -1, depthProperties);

    // Outputs
    addOutput("output image", PlugType::Image);

    // Help
    addHelpMessage("addWeighted",
                   CvUtils::makeUrl({"d2", "de8", "group__core__array"}, "gafafb2513349db3bcff51f54ee5592a19"));
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d5", "dc4", "tutorial_adding_images"}));
}

Properties AddWeightedProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;
    cv::addWeighted(inputs["input image 1"].value<cv::Mat>(),
                    inputs["alpha"].toDouble(),
                    inputs["input image 2"].value<cv::Mat>(),
                    inputs["beta"].toDouble(),
                    inputs["gamma"].toDouble(),
                    outputImage,
                    inputs["depth"].toInt());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


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
    Properties coefficientsProperties;
    coefficientsProperties.insert("minimum", -CvConstants::defaultDoubleMax);
    coefficientsProperties.insert("singleStep", 0.1);

    addInput("input image 1", PlugType::Image);
    addInput("alpha", PlugType::Double, 0.5, coefficientsProperties);
    addInput("input image 2", PlugType::Image);
    addInput("beta", PlugType::Double, 0.5, coefficientsProperties);
    addInput("gamma", PlugType::Double, 0.0, coefficientsProperties);

    addOutput("output image", PlugType::Image);
}

Properties AddWeightedProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;
    cv::addWeighted(inputs["input image 1"].value<cv::Mat>(),
                    inputs["alpha"].toDouble(),
                    inputs["input image 2"].value<cv::Mat>(),
                    inputs["beta"].toDouble(),
                    inputs["gamma"].toDouble(),
                    outputImage);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


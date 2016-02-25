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

#include "kernelprocessor.h"

#include "global/cvutils.h"


KernelProcessor::KernelProcessor()
{
    /*Properties sizeProperties;
    sizeProperties.insert("decimals", 0);
    sizeProperties.insert("minimum", 1.0);
    sizeProperties.insert("maximum", 10.0);

    addInput("width", PlugType::Double, 3.0, sizeProperties);
    addInput("height", PlugType::Double, 3.0, sizeProperties);*/

    double initData[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    cv::Mat initMat = cv::Mat(3, 3, CV_64FC1, initData).clone();
    addInput("definition", PlugType::KernelDefinition, QVariant::fromValue(initMat));

    addOutput("kernel", PlugType::Kernel);
}

Properties KernelProcessor::processImpl(const Properties &inputs)
{
    Properties properties;
    properties.insert("kernel", inputs["definition"]);
    return properties;
}

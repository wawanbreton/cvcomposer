// Copyright 2022 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "meanprocessor.h"

#include "global/cvutils.h"


MeanProcessor::MeanProcessor()
{
    // Inputs
    addInput("image", PlugType::Image, ProcessorListType::None);

    // Outputs
    addOutput("mean", PlugType::Double, ProcessorListType::Custom);

    // Help
    addHelpMessage("mean",
                   CvUtils::makeUrl({"d2", "de8", "group__core__array"}, "ga191389f8a0e58180bb13a727782cd461"),
                   HelpMessageType::Function);
}

Properties MeanProcessor::processImpl(const Properties &inputs)
{
    Properties outputs;

    cv::Mat image = inputs["image"].value<cv::Mat>();
    cv::Scalar mean = cv::mean(image);

    QList<QVariant> means;
    for(int i = 0 ; i < image.channels() ; ++i)
    {
        means.append(mean[i]);
    }

    outputs.insert("mean", means);
    return outputs;
}


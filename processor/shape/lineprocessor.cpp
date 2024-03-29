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

#include "lineprocessor.h"

#include "global/cvutils.h"
#include "model/segment.h"


LineProcessor::LineProcessor()
{
    // Inputs
    addInput("point 1", PlugType::Point);
    addInput("point 2", PlugType::Point);

    // Outputs
    addOutput("line", PlugType::Segment);

    // Help
    addHelpMessage("Point",
                   CvUtils::makeUrl({"db", "d4e", "classcv_1_1Point__"}),
                   HelpMessageType::Class);
}

Properties LineProcessor::processImpl(const Properties &inputs)
{
    Segment line;
    line.point1 = inputs["point 1"].value<cv::Point>();
    line.point2 = inputs["point 2"].value<cv::Point>();

    Properties outputs;
    outputs.insert("line", QVariant::fromValue(line));
    return outputs;
}


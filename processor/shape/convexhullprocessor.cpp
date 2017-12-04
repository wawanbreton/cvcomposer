// Copyright 2017 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "convexhullprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"
#include "model/contour.h"


ConvexHullProcessor::ConvexHullProcessor()
{
    // Inputs
    addInput("contour", PlugType::Contour, ProcessorListType::Simple);
    addInput("clockwise", PlugType::Boolean);

    // Outputs
    addOutput("hull", PlugType::Contour, ProcessorListType::Simple);

    // Help
    addHelpMessage("convexHull",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "ga014b28e56cb8854c0de4a211cb2be656"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d7", "d1d", "tutorial_hull"}),
                   HelpMessageType::Tutorial);
}

Properties ConvexHullProcessor::processImpl(const Properties &inputs)
{
    Contour hull;

    cv::convexHull(inputs["contour"].value<Contour>(),
                   hull,
                   inputs["clockwise"].toBool());

    Properties outputs;
    outputs.insert("hull", QVariant::fromValue(hull));
    return outputs;
}


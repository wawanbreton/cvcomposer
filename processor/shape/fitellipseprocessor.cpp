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

#include "fitellipseprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"
#include "model/contour.h"


FitEllipseProcessor::FitEllipseProcessor()
{
    // Inputs
    addInput("contour", PlugType::Contour, ProcessorListType::Simple);

    // Outputs
    addOutput("ellipse", PlugType::RotatedRectangle, ProcessorListType::Simple);

    // Help
    addHelpMessage("fitEllipse",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "gaf259efaad93098103d6c27b9e4900ffa"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"de", "d62", "tutorial_bounding_rotated_ellipses"}),
                   HelpMessageType::Tutorial);
}

Properties FitEllipseProcessor::processImpl(const Properties &inputs)
{
    cv::RotatedRect ellipse = cv::fitEllipse(inputs["contour"].value<Contour>());

    Properties outputs;
    outputs.insert("ellipse", QVariant::fromValue(ellipse));
    return outputs;
}


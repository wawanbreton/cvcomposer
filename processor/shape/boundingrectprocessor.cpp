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

#include "boundingrectprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"
#include "model/contour.h"


BoundingRectProcessor::BoundingRectProcessor()
{
    // Inputs
    addInput("contour", PlugType::Contour, ProcessorListType::Simple);
    addInput("rotated", PlugType::Boolean);

    // Outputs
    addOutput("bounding box",
              PlugType::Rectangle | PlugType::RotatedRectangle,
              ProcessorListType::Simple);

    // Help
    addHelpMessage("boundingRect",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "gacb413ddce8e48ff3ca61ed7cf626a366"),
                   HelpMessageType::Function);
    addHelpMessage("minAreaRect",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "ga3d476a3417130ae5154aea421ca7ead9"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial bounding boxes",
                   CvUtils::makeUrl({"da", "d0c", "tutorial_bounding_rects_circles"}),
                   HelpMessageType::Tutorial);
    addHelpMessage("tutorial contour features",
                   CvUtils::makeUrl({"dd", "d49", "tutorial_py_contour_features"}),
                   HelpMessageType::Tutorial);
}

Properties BoundingRectProcessor::processImpl(const Properties &inputs)
{
    Contour contour = inputs["contour"].value<Contour>();
    bool rotated = inputs["rotated"].toBool();

    Properties outputs;
    if(rotated)
    {
        outputs.insert("bounding box", QVariant::fromValue(cv::minAreaRect(contour)));
    }
    else
    {
        outputs.insert("bounding box", QVariant::fromValue(cv::boundingRect(contour)));
    }
    return outputs;
}


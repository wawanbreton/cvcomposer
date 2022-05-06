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

#include "minenclosingtriangleprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "model/contour.h"
#include "model/triangle.h"
#include "global/cvutils.h"


MinEnclosingTriangleProcessor::MinEnclosingTriangleProcessor()
{
    // Inputs
    addInput("points", PlugType::Contour, ProcessorListType::Simple);

    // Outputs
    addOutput("triangle", PlugType::Triangle, ProcessorListType::Simple);
    addOutput("area",     PlugType::Double, ProcessorListType::Simple);

    // Help
    addHelpMessage("minEnclosingTriangle",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "ga1513e72f6bbdfc370563664f71e0542f"),
                   HelpMessageType::Function);
}

Properties MinEnclosingTriangleProcessor::processImpl(const Properties &inputs)
{
    std::vector<cv::Point2f> triangle;
    double area = cv::minEnclosingTriangle(inputs["points"].value<Contour>(), triangle);

    Properties outputs;
    outputs.insert("triangle", QVariant::fromValue(Triangle{triangle.at(0), triangle.at(1), triangle.at(2)}));
    outputs.insert("area", area);
    return outputs;
}

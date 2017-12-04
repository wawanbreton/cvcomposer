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

#include "circleprocessor.h"

#include "global/cvutils.h"
#include "global/cvconstants.h"
#include "model/circle.h"


CircleProcessor::CircleProcessor()
{
    // Inputs
    addInput("center", PlugType::Point);

    Properties radiusProperties;
    radiusProperties.insert("decimals", 0);
    addInput("radius", PlugType::Double, CvConstants::defaultShapeSide / 2, radiusProperties);

    // Outputs
    addOutput("circle", PlugType::Circle);
}

Properties CircleProcessor::processImpl(const Properties &inputs)
{
    Circle circle;
    circle.center = inputs["center"].value<cv::Point>();
    circle.radius = inputs["radius"].toInt();

    Properties outputs;
    outputs.insert("circle", QVariant::fromValue(circle));
    return outputs;
}

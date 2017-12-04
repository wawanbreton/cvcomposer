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

#include "rectangleprocessor.h"

#include "global/cvutils.h"


RectangleProcessor::RectangleProcessor()
{
    // Inputs
    addInput("position", PlugType::Point, QVariant::fromValue(cv::Point(0, 0)));

    QList<QPair<QString, QVariant> > anchorValues;
    anchorValues << QPair<QString, QVariant>("Top left", TopLeft);
    anchorValues << QPair<QString, QVariant>("Top Right", TopRight);
    anchorValues << QPair<QString, QVariant>("Bottom left", BottomLeft);
    anchorValues << QPair<QString, QVariant>("BottomRight", BottomRight);
    anchorValues << QPair<QString, QVariant>("Center", Center);
    addEnumerationInput("anchor", anchorValues, TopLeft);

    addInput("size", PlugType::Size, QVariant::fromValue(cv::Size(10, 10)));

    // Outputs
    addOutput("rectangle", PlugType::Rectangle);

    // Help
    addHelpMessage("Rect",
                   CvUtils::makeUrl({"d2", "d44", "classcv_1_1Rect__"}),
                   HelpMessageType::Class);
}

Properties RectangleProcessor::processImpl(const Properties &inputs)
{
    cv::Point position = inputs["position"].value<cv::Point>();
    Anchor anchor = Anchor(inputs["anchor"].toInt());
    cv::Size size = inputs["size"].value<cv::Size>();

    switch(anchor)
    {
        case TopLeft:
            break;
        case TopRight:
            position.x -= size.width;
            break;
        case BottomLeft:
            position.y -= size.height;
            break;
        case BottomRight:
            position.x -= size.width;
            position.y -= size.height;
            break;
        case Center:
            position.x -= size.width / 2;
            position.y -= size.height / 2;
            break;
    }

    cv::Rect rectangle(position, size);

    Properties outputs;
    outputs.insert("rectangle", QVariant::fromValue(rectangle));
    return outputs;
}

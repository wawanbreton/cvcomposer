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

#include "drawshapeprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"
#include "model/circle.h"
#include "model/ellipse.h"
#include "model/segment.h"
#include "model/contour.h"
#include "model/triangle.h"
#include "model/line.h"


DrawShapeProcessor::DrawShapeProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);
    addInput("shape",
             PlugType::Circle |
             PlugType::Rectangle |
             PlugType::Segment |
             PlugType::Ellipse |
             PlugType::Contour |
             PlugType::RotatedRectangle |
             PlugType::Triangle |
             PlugType::Line,
             ProcessorListType::Custom);
    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(255, 255, 255, 255)));

    Properties thicknessProperties;
    thicknessProperties.insert("minimum", -1);
    thicknessProperties.insert("decimals", 0);
    addInput("thickness", PlugType::Double, 1, thicknessProperties);

    addEnumerationInput("line type", CvUtils::makeLineTypeValues(), 8);

    Properties shiftProperties;
    shiftProperties.insert("decimals", 0);
    addInput("shift", PlugType::Double, 0, shiftProperties);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    addHelpMessage("circle",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "gaf10604b069374903dbd0f0488cb43670"),
                   HelpMessageType::Function);
    addHelpMessage("rectangle",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "ga346ac30b5c74e9b5137576c9ee9e0e8c"),
                   HelpMessageType::Function);
    addHelpMessage("line",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "ga7078a9fae8c7e7d13d24dac2520ae4a2"),
                   HelpMessageType::Function);
    addHelpMessage("ellipse",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "ga28b2267d35786f5f890ca167236cbc69"),
                   HelpMessageType::Function);
    addHelpMessage("drawContours",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "ga746c0625f1781f1ffc9056259103edbc"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"dc", "da5", "tutorial_py_drawing_functions"}),
                   HelpMessageType::Tutorial);
}

Properties DrawShapeProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    QList<QVariant> shapes = inputs["shape"].value<QList<QVariant>>();
    cv::Scalar color = inputs["color"].value<cv::Scalar>();
    int thickness = inputs["thickness"].toInt();
    int lineType = inputs["line type"].toInt();
    int shift = inputs["shift"].toInt();

    cv::Mat outputImage = inputImage.clone();

    foreach(const QVariant &shape, shapes)
    {
        if(shape.userType() == qMetaTypeId<Circle>())
        {
            Circle circle = shape.value<Circle>();
            cv::circle(outputImage, circle.center, circle.radius, color, thickness, lineType, shift);
        }
        else if(shape.userType() == qMetaTypeId<cv::Rect>())
        {
            cv::Rect rect = shape.value<cv::Rect>();
            cv::rectangle(outputImage, rect, color, thickness, lineType, shift);
        }
        else if(shape.userType() == qMetaTypeId<Segment>())
        {
            Segment line = shape.value<Segment>();
            cv::line(outputImage, line.point1, line.point2, color, thickness, lineType, shift);
        }
        else if(shape.userType() == qMetaTypeId<Ellipse>())
        {
            Ellipse ellipse = shape.value<Ellipse>();
            cv::ellipse(outputImage,
                        ellipse.center,
                        ellipse.axes,
                        ellipse.angle,
                        ellipse.startAngle,
                        ellipse.endAngle,
                        color,
                        thickness,
                        lineType,
                        shift);
        }
        else if(shape.userType() == qMetaTypeId<Contour>())
        {
            std::vector<Contour> contours;
            contours.push_back(shape.value<Contour>());
            cv::drawContours(outputImage, contours, -1, color, thickness, lineType);
        }
        else if(shape.userType() == qMetaTypeId<cv::RotatedRect>())
        {
            cv::RotatedRect rect = shape.value<cv::RotatedRect>();

            cv::Point2f points[4];
            rect.points(points);

            for(int i = 0 ; i < 4 ; ++i)
            {
                cv::line(outputImage, points[i], points[(i + 1) % 4], color, thickness, lineType, shift);
            }
        }
        else if(shape.userType() == qMetaTypeId<Triangle>())
        {
            Triangle triangle = shape.value<Triangle>();

            cv::line(outputImage, triangle.p1, triangle.p2, color, thickness, lineType, shift);
            cv::line(outputImage, triangle.p2, triangle.p3, color, thickness, lineType, shift);
            cv::line(outputImage, triangle.p3, triangle.p1, color, thickness, lineType, shift);
        }
        else if(shape.userType() == qMetaTypeId<Line>())
        {
            Line line = shape.value<Line>();

            cv::Point2f direction;
            direction.x = std::cos(line.angle);
            direction.y = std::sin(line.angle);

            double expand = std::max(inputImage.rows, inputImage.cols) * 4;
            direction *= expand;

            cv::line(outputImage, line.point + direction, line.point - direction, color, thickness, lineType, shift);
        }
    }

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


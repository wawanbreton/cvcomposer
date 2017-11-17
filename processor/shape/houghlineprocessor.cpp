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

#include "houghlineprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"
#include "model/line.h"


HoughLineProcessor::HoughLineProcessor()
{
    addInput("input image", PlugType::Image);

    Properties distanceProperties;
    distanceProperties.insert("decimals", 3);
    distanceProperties.insert("minimum", 0.001);
    distanceProperties.insert("singleStep", 0.1);
    distanceProperties.insert("suffix", " px");
    addInput("distance resolution", PlugType::Double, 1, distanceProperties);

    Properties angleProperties;
    angleProperties.insert("decimals", 1);
    angleProperties.insert("minimum", 1);
    angleProperties.insert("maximum", 180);
    angleProperties.insert("suffix", " °");
    addInput("angle resolution", PlugType::Double, 1, angleProperties);

    Properties thresholdProperties;
    angleProperties.insert("decimals", 0);
    angleProperties.insert("minimum", 1);
    angleProperties.insert("maximum", 256);
    addInput("threshold", PlugType::Double, 128, thresholdProperties);

    Properties multiScaleProperties;
    multiScaleProperties.insert("decimals", 0);
    multiScaleProperties.insert("minimum", 0);
    addInput("multi-scale angle", PlugType::Double, 0, multiScaleProperties);
    addInput("multi-scale distance", PlugType::Double, 0, multiScaleProperties);

    Properties minMaxProperties;
    minMaxProperties.insert("decimals", 1);
    minMaxProperties.insert("minimum", 0);
    minMaxProperties.insert("maximum", 180);
    minMaxProperties.insert("suffix", " °");
    addInput("min angle", PlugType::Double, 0, minMaxProperties);
    addInput("max angle", PlugType::Double, 180, minMaxProperties);

    addOutput("lines", PlugType::Line, ProcessorListType::Custom);
}

Properties HoughLineProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    std::vector<cv::Vec2f> lines;

    cv::HoughLines(inputImage,
                   lines,
                   inputs["distance resolution"].toDouble(),
                   CvUtils::degToRad(inputs["angle resolution"].toDouble()),
                   inputs["threshold"].toInt(),
                   inputs["multi-scale angle"].toInt(),
                   inputs["multi-scale distance"].toInt(),
                   CvUtils::degToRad(inputs["min angle"].toDouble()),
                   CvUtils::degToRad(inputs["max angle"].toDouble()));

    Properties outputs;

    QList<QVariant> outputLines;

    for(const cv::Vec2f &parameterizedLine : lines)
    {
        const double &rho = parameterizedLine[0];
        const double &theta = parameterizedLine[1];

        cv::Point2f deltaVector;
        deltaVector.x = -sin(theta);
        deltaVector.y = cos(theta);

        cv::Point2f lineCenter;
        lineCenter.x = rho * deltaVector.y;
        lineCenter.y = rho * -deltaVector.x;

        double expand = qMax(inputImage.rows, inputImage.cols) * 2;
        deltaVector.x *= expand;
        deltaVector.y *= expand;

        cv::Point2f p1 = lineCenter + deltaVector;
        cv::Point2f p2 = lineCenter - deltaVector;

        Line line;
        line.point1.x = qRound(p1.x);
        line.point1.y = qRound(p1.y);
        line.point2.x = qRound(p2.x);
        line.point2.y = qRound(p2.y);

        outputLines << QVariant::fromValue(line);
    }

    outputs.insert("lines", QVariant::fromValue(outputLines));

    return outputs;
}


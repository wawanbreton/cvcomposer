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

#include "houghlineprobaprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QtMath>

#include "global/cvutils.h"
#include "model/segment.h"


HoughLineProbaProcessor::HoughLineProbaProcessor()
{
    // Inputs
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

    Properties minMaxProperties;
    minMaxProperties.insert("decimals", 1);
    minMaxProperties.insert("minimum", 0);
    addInput("min length", PlugType::Double, 0, minMaxProperties);
    addInput("max gap",    PlugType::Double, 0, minMaxProperties);

    // Outputs
    addOutput("lines", PlugType::Segment, ProcessorListType::Custom);

    // Help
    addHelpMessage("HoughLinesP",
                   CvUtils::makeUrl({"dd", "d1a", "group__imgproc__feature"}, "ga8618180a5948286384e3b7ca02f6feeb"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"d6", "d10", "tutorial_py_houghlines"}),
                   HelpMessageType::Tutorial);
}

Properties HoughLineProbaProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    std::vector<cv::Vec4f> lines;

    cv::HoughLinesP(inputImage,
                    lines,
                    inputs["distance resolution"].toDouble(),
                    qDegreesToRadians(inputs["angle resolution"].toDouble()),
                    inputs["threshold"].toInt(),
                    inputs["min length"].toDouble(),
                    inputs["max gap"].toDouble());

    Properties outputs;

    QList<QVariant> outputLines;

    for(const cv::Vec4f &linePoints : lines)
    {
        Segment line;
        line.point1.x = linePoints[0];
        line.point1.y = linePoints[1];
        line.point2.x = linePoints[2];
        line.point2.y = linePoints[3];

        outputLines << QVariant::fromValue(line);
    }

    outputs.insert("lines", QVariant::fromValue(outputLines));

    return outputs;
}


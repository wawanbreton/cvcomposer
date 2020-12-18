// Copyright 2019 Erwan MATHIEU <wawanbreton@gmail.com>
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

#include "floodfillprocessor.h"

#include <opencv2/imgproc.hpp>

#include "global/cvutils.h"


FloodFillProcessor::FloodFillProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);
    addInput("seed point", PlugType::Point);
    addInput("new value", PlugType::Color);
    addInput("lower difference", PlugType::Double);
    addInput("upper difference", PlugType::Double);
    addInput("fixed range", PlugType::Boolean);

    QList<QPair<QString, QVariant> > connectivity;
    connectivity << QPair<QString, QVariant>("4 neighbors", 4);
    connectivity << QPair<QString, QVariant>("8 neighbors", 8);
    addEnumerationInput("connectivity", connectivity);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("floodFill",
                   CvUtils::makeUrl({"d7", "d1b", "group__imgproc__misc"},
                                    "ga366aae45a6c1289b341d140839f18717"),
                   HelpMessageType::Function);
}

Properties FloodFillProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>().clone();

    int flags = inputs["connectivity"].value<int>();
    if(inputs["fixed range"].toBool())
    {
        flags |= cv::FLOODFILL_FIXED_RANGE;
    }

    double lowerDifference = inputs["lower difference"].value<double>();
    double upperDifference = inputs["upper difference"].value<double>();
    cv::Scalar lowDif(lowerDifference, lowerDifference, lowerDifference);
    cv::Scalar upDif(upperDifference, upperDifference, upperDifference);

    cv::floodFill(inputImage,
                  inputs["seed point"].value<cv::Point>(),
                  inputs["new value"].value<cv::Scalar>(),
                  nullptr,
                  lowDif,
                  upDif,
                  flags);

    Properties properties;
    properties.insert("output image", QVariant::fromValue(inputImage));
    return properties;
}


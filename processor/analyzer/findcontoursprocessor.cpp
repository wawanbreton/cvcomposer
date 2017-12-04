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

#include "findcontoursprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"
#include "model/contour.h"


FindContoursProcessor::FindContoursProcessor()
{
    // Inputs
    addInput("image", PlugType::Image);

    QList<QPair<QString, QVariant> > modes;
    modes << QPair<QString, QVariant>("External", CV_RETR_EXTERNAL);
    modes << QPair<QString, QVariant>("List", CV_RETR_LIST);
    modes << QPair<QString, QVariant>("Connected components", CV_RETR_CCOMP);
    modes << QPair<QString, QVariant>("Tree", CV_RETR_TREE);
    addEnumerationInput("mode", modes, CV_RETR_EXTERNAL);

    QList<QPair<QString, QVariant> > methods;
    methods << QPair<QString, QVariant>("None", CV_CHAIN_APPROX_NONE);
    methods << QPair<QString, QVariant>("Simple", CV_CHAIN_APPROX_SIMPLE);
    methods << QPair<QString, QVariant>("Teh-Chin L1", CV_CHAIN_APPROX_TC89_L1);
    methods << QPair<QString, QVariant>("Teh-Chin KCOS", CV_CHAIN_APPROX_TC89_KCOS);
    addEnumerationInput("approximation", methods, CV_CHAIN_APPROX_NONE);

    addInput("offset", PlugType::Point);

    // Outputs
    addOutput("contours", PlugType::Contour, ProcessorListType::Custom);


    // Help
    addHelpMessage("findContours",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "ga17ed9f5d79ae97bd4c7cf18403e1689a"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"df", "d0d", "tutorial_find_contours"}),
                   HelpMessageType::Tutorial);
}

Properties FindContoursProcessor::processImpl(const Properties &inputs)
{
    std::vector<std::vector<cv::Point> > contours;

    cv::findContours(inputs["image"].value<cv::Mat>(),
                     contours,
                     inputs["mode"].toInt(),
                     inputs["approximation"].toInt(),
                     inputs["offset"].value<cv::Point>());

    QList<QVariant> convertedContours;

    for(const std::vector<cv::Point> &contour : contours)
    {
        convertedContours << QVariant::fromValue(contour);
    }

    Properties outputs;
    outputs.insert("contours", QVariant::fromValue(convertedContours));
    return outputs;
}


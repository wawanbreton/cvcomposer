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

#include "fitlineprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"
#include "model/contour.h"
#include "model/line.h"


FitLineProcessor::FitLineProcessor()
{
    // Inputs
    addInput("contour", PlugType::Contour, ProcessorListType::Simple);

    QList<QPair<QString, QVariant> > types;
    types << QPair<QString, QVariant>("L2",     cv::DIST_L2);
    types << QPair<QString, QVariant>("L1",     cv::DIST_L1);
    types << QPair<QString, QVariant>("L12",    cv::DIST_L12);
    types << QPair<QString, QVariant>("Fair",   cv::DIST_FAIR);
    types << QPair<QString, QVariant>("Welsch", cv::DIST_WELSCH);
    types << QPair<QString, QVariant>("Huber",  cv::DIST_HUBER);
    addEnumerationInput("type", types);

    addInput("param", PlugType::Double);
    addInput("reps", PlugType::Double);
    addInput("aeps", PlugType::Double);

    // Outputs
    addOutput("line", PlugType::Line, ProcessorListType::Simple);

    // Help
    addHelpMessage("fitLine",
                   CvUtils::makeUrl({"d3", "dc0", "group__imgproc__shape"}, "gaf849da1fdafa67ee84b1e9a23b93f91f"),
                   HelpMessageType::Function);
}

Properties FitLineProcessor::processImpl(const Properties &inputs)
{
    cv::Vec4f fittedLine;
    cv::fitLine(inputs["contour"].value<Contour>(),
                fittedLine,
                inputs["type"].toInt(),
                inputs["param"].toDouble(),
                inputs["reps"].toDouble(),
                inputs["aeps"].toDouble());

    Line line;
    line.point = cv::Point2f(fittedLine[2], fittedLine[3]);
    line.angle = std::atan2(fittedLine[1], fittedLine[0]);

    Properties outputs;
    outputs.insert("line", QVariant::fromValue(line));
    return outputs;
}


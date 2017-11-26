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

#include "drawtextprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


DrawTextProcessor::DrawTextProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image);

    // TODO : manage multiple texts with multiples positions
    addInput("text", PlugType::String);
    addInput("position", PlugType::Point);

    QList<QPair<QString, QVariant> > fontValues;
    fontValues << QPair<QString, QVariant>("Simplex", cv::FONT_HERSHEY_SIMPLEX);
    fontValues << QPair<QString, QVariant>("Plain", cv::FONT_HERSHEY_PLAIN);
    fontValues << QPair<QString, QVariant>("Duplex", cv::FONT_HERSHEY_DUPLEX);
    fontValues << QPair<QString, QVariant>("Complex", cv::FONT_HERSHEY_COMPLEX);
    fontValues << QPair<QString, QVariant>("Triplex", cv::FONT_HERSHEY_TRIPLEX);
    fontValues << QPair<QString, QVariant>("Complex small", cv::FONT_HERSHEY_COMPLEX_SMALL);
    fontValues << QPair<QString, QVariant>("Script simplex", cv::FONT_HERSHEY_SCRIPT_SIMPLEX);
    fontValues << QPair<QString, QVariant>("Script complex", cv::FONT_HERSHEY_SCRIPT_COMPLEX);
    addEnumerationInput("font", fontValues, cv::FONT_HERSHEY_SIMPLEX);

    addInput("italic", PlugType::Boolean, false);

    Properties scaleProperties;
    scaleProperties.insert("singleStep", 0.1);
    addInput("scale", PlugType::Double, 1.0, scaleProperties);

    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(255, 255, 255, 255)));

    Properties thicknessProperties;
    thicknessProperties.insert("minimum", 1);
    thicknessProperties.insert("decimals", 0);
    addInput("thickness", PlugType::Double, 1, thicknessProperties);

    addEnumerationInput("line type", CvUtils::makeLineTypeValues(), 8);

    QList<QPair<QString, QVariant> > originValues;
    originValues << QPair<QString, QVariant>("Top-left", false);
    originValues << QPair<QString, QVariant>("Bottom-left", true);
    addEnumerationInput("origin", originValues, false);

    // Outputs
    addOutput("output image", PlugType::Image);

    // Help
    addHelpMessage("putText",
                   CvUtils::makeUrl({"d6", "d6e", "group__imgproc__draw"}, "ga5126f47f883d730f633d74f07456c576"));
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"dc", "da5", "tutorial_py_drawing_functions"}));
}

Properties DrawTextProcessor::processImpl(const Properties &inputs)
{
    int fontFace = inputs["font"].toInt();
    if(inputs["italic"].toBool())
    {
        fontFace |= cv::FONT_ITALIC;
    }

    cv::Mat outputImage = inputs["input image"].value<cv::Mat>().clone();
    cv::putText(outputImage,
                inputs["text"].toString().toStdString(),
                inputs["position"].value<cv::Point>(),
                fontFace,
                inputs["scale"].toDouble(),
                inputs["color"].value<cv::Scalar>(),
                inputs["thickness"].toInt(),
                inputs["line type"].toInt(),
                inputs["origin"].toBool());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


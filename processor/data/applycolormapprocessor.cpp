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

#include "applycolormapprocessor.h"

#include <opencv2/imgproc.hpp>

#include "global/cvutils.h"


ApplyColorMapProcessor::ApplyColorMapProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    QList<QPair<QString, QVariant> > colorMaps;
    colorMaps << QPair<QString, QVariant>("Automn",             cv::COLORMAP_AUTUMN);
    colorMaps << QPair<QString, QVariant>("Bone",               cv::COLORMAP_BONE);
    colorMaps << QPair<QString, QVariant>("Jet",                cv::COLORMAP_JET);
    colorMaps << QPair<QString, QVariant>("Rainbow",            cv::COLORMAP_RAINBOW);
    colorMaps << QPair<QString, QVariant>("Ocean",              cv::COLORMAP_OCEAN);
    colorMaps << QPair<QString, QVariant>("Summer",             cv::COLORMAP_SUMMER);
    colorMaps << QPair<QString, QVariant>("Spring",             cv::COLORMAP_SPRING);
    colorMaps << QPair<QString, QVariant>("Cool",               cv::COLORMAP_COOL);
    colorMaps << QPair<QString, QVariant>("HSV",                cv::COLORMAP_HSV);
    colorMaps << QPair<QString, QVariant>("Pink",               cv::COLORMAP_PINK);
    colorMaps << QPair<QString, QVariant>("Hot",                cv::COLORMAP_HOT);
    colorMaps << QPair<QString, QVariant>("Magma",              cv::COLORMAP_MAGMA);
    colorMaps << QPair<QString, QVariant>("Inferno",            cv::COLORMAP_INFERNO);
    colorMaps << QPair<QString, QVariant>("Plasma",             cv::COLORMAP_PLASMA);
    colorMaps << QPair<QString, QVariant>("Viridis",            cv::COLORMAP_VIRIDIS);
    colorMaps << QPair<QString, QVariant>("Cividis",            cv::COLORMAP_CIVIDIS);
    colorMaps << QPair<QString, QVariant>("Twilight",           cv::COLORMAP_TWILIGHT);
    colorMaps << QPair<QString, QVariant>("Twilight (shifted)", cv::COLORMAP_TWILIGHT_SHIFTED);
    colorMaps << QPair<QString, QVariant>("Turbo",              cv::COLORMAP_TURBO);

    addEnumerationInput("color map", colorMaps);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("applyColorMap",
                   CvUtils::makeUrl({"d3", "d50", "group__imgproc__colormap"},
                                    "gadf478a5e5ff49d8aa24e726ea6f65d15"),
                   HelpMessageType::Function);
}

Properties ApplyColorMapProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat result;
    cv::applyColorMap(inputImage, result, inputs["color map"].toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(result));
    return properties;
}


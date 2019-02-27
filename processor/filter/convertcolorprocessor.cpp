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

#include "convertcolorprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"

// Use macros to avoid typos, as the variables are all very likely
#define MAKE_TYPE(type) conversionTypes << makeConversionType(#type, cv::type);
#define MAKE_TYPE_HELP(type, help) conversionTypes << makeConversionType(#type, cv::type, help);


ConvertColorProcessor::ConvertColorProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    QList<QPair<QString, QVariant>> conversionTypes;
    MAKE_TYPE_HELP(COLOR_BGR2BGRA, "add alpha")
    MAKE_TYPE_HELP(COLOR_RGB2RGBA, "add alpha")

    MAKE_TYPE_HELP(COLOR_BGRA2BGR, "remove alpha")
    MAKE_TYPE_HELP(COLOR_RGBA2RGB, "remove alpha")

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2RGBA)
    MAKE_TYPE(COLOR_RGB2BGRA)

    MAKE_TYPE(COLOR_RGBA2BGR)
    MAKE_TYPE(COLOR_BGRA2RGB)

    MAKE_TYPE(COLOR_BGR2RGB)
    MAKE_TYPE(COLOR_RGB2BGR)

    MAKE_TYPE(COLOR_BGRA2RGBA)
    MAKE_TYPE(COLOR_RGBA2BGRA)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2GRAY)
    MAKE_TYPE(COLOR_RGB2GRAY)

    MAKE_TYPE(COLOR_GRAY2BGR)
    MAKE_TYPE(COLOR_GRAY2RGB)

    MAKE_TYPE(COLOR_GRAY2BGRA)
    MAKE_TYPE(COLOR_GRAY2RGBA)

    MAKE_TYPE(COLOR_BGRA2GRAY)
    MAKE_TYPE(COLOR_RGBA2GRAY)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2BGR565)
    MAKE_TYPE(COLOR_RGB2BGR565)

    MAKE_TYPE(COLOR_BGR5652BGR)
    MAKE_TYPE(COLOR_BGR5652RGB)

    MAKE_TYPE(COLOR_BGRA2BGR565)
    MAKE_TYPE(COLOR_RGBA2BGR565)

    MAKE_TYPE(COLOR_BGR5652BGRA)
    MAKE_TYPE(COLOR_BGR5652RGBA)

    MAKE_TYPE(COLOR_GRAY2BGR565)
    MAKE_TYPE(COLOR_BGR5652GRAY)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2BGR555)
    MAKE_TYPE(COLOR_RGB2BGR555)

    MAKE_TYPE(COLOR_BGR5552BGR)
    MAKE_TYPE(COLOR_BGR5552RGB)

    MAKE_TYPE(COLOR_BGRA2BGR555)
    MAKE_TYPE(COLOR_RGBA2BGR555)

    MAKE_TYPE(COLOR_BGR5552BGRA)
    MAKE_TYPE(COLOR_BGR5552RGBA)

    MAKE_TYPE(COLOR_GRAY2BGR555)
    MAKE_TYPE(COLOR_BGR5552GRAY)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2XYZ)
    MAKE_TYPE(COLOR_RGB2XYZ)

    MAKE_TYPE(COLOR_XYZ2BGR)
    MAKE_TYPE(COLOR_XYZ2RGB)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2YCrCb)
    MAKE_TYPE(COLOR_RGB2YCrCb)

    MAKE_TYPE(COLOR_YCrCb2BGR)
    MAKE_TYPE(COLOR_YCrCb2RGB)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2HSV)
    MAKE_TYPE(COLOR_RGB2HSV)

    MAKE_TYPE(COLOR_BGR2HSV_FULL)
    MAKE_TYPE(COLOR_RGB2HSV_FULL)

    MAKE_TYPE(COLOR_HSV2BGR)
    MAKE_TYPE(COLOR_HSV2RGB)

    MAKE_TYPE(COLOR_HSV2BGR_FULL)
    MAKE_TYPE(COLOR_HSV2RGB_FULL)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2Lab)
    MAKE_TYPE(COLOR_RGB2Lab)

    MAKE_TYPE(COLOR_LBGR2Lab)
    MAKE_TYPE(COLOR_LRGB2Lab)

    MAKE_TYPE(COLOR_Lab2BGR)
    MAKE_TYPE(COLOR_Lab2RGB)

    MAKE_TYPE(COLOR_Lab2LBGR)
    MAKE_TYPE(COLOR_Lab2LRGB)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2Luv)
    MAKE_TYPE(COLOR_RGB2Luv)

    MAKE_TYPE(COLOR_LBGR2Luv)
    MAKE_TYPE(COLOR_LRGB2Luv)

    MAKE_TYPE(COLOR_Luv2BGR)
    MAKE_TYPE(COLOR_Luv2RGB)

    MAKE_TYPE(COLOR_Luv2LBGR)
    MAKE_TYPE(COLOR_Luv2LRGB)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2HLS)
    MAKE_TYPE(COLOR_RGB2HLS)

    MAKE_TYPE(COLOR_BGR2HLS_FULL)
    MAKE_TYPE(COLOR_RGB2HLS_FULL)

    MAKE_TYPE(COLOR_HLS2BGR)
    MAKE_TYPE(COLOR_HLS2RGB)

    MAKE_TYPE(COLOR_HLS2BGR_FULL)
    MAKE_TYPE(COLOR_HLS2RGB_FULL)

    conversionTypes << QPair<QString, QVariant>(QString(), QVariant());

    MAKE_TYPE(COLOR_BGR2YUV)
    MAKE_TYPE(COLOR_RGB2YUV)

    MAKE_TYPE(COLOR_YUV2BGR)
    MAKE_TYPE(COLOR_YUV2RGB)

    addEnumerationInput("type", conversionTypes);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("cvtColor",
                   CvUtils::makeUrl({"d7", "d1b", "group__imgproc__misc"}, "ga397ae87e1288a81d2363b61574eb8cab"),
                   HelpMessageType::Function);
    addHelpMessage("color conversions",
                   CvUtils::makeUrl({"de", "d25", "imgproc_color_conversions"}),
                   HelpMessageType::Tutorial);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"df", "d9d", "tutorial_py_colorspaces"}),
                   HelpMessageType::Tutorial);
}

Properties ConvertColorProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    int type = inputs["type"].toInt();
    cv::Mat outputImage;

    cv::cvtColor(inputImage, outputImage, type);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}

QPair<QString, QVariant> ConvertColorProcessor::makeConversionType(const QString &rawString,
                                                                   int value,
                                                                   const QString &help)
{
    QString text = help.isEmpty() ? "%1 => %2" : "%1 => %2 (%3)";
    QStringList tokens = rawString.mid(6).split('2');
    text = text.arg(tokens[0]);
    text = text.arg(tokens[1]);
    if(!help.isEmpty())
    {
        text = text.arg(help);
    }

    return QPair<QString, QVariant>(text, value);
}


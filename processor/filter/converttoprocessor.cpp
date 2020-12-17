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

#include "converttoprocessor.h"

#include "global/cvconstants.h"
#include "global/cvutils.h"


ConvertToProcessor::ConvertToProcessor()
{
    // Inputs
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    QList<QPair<QString, QVariant> > colorMaps;
    colorMaps << QPair<QString, QVariant>("Unchanged", -1);
    colorMaps << QPair<QString, QVariant>("8bit unsigned",  CV_8U);
    colorMaps << QPair<QString, QVariant>("8bit signed",    CV_8S);
    colorMaps << QPair<QString, QVariant>("16bit unsigned", CV_16U);
    colorMaps << QPair<QString, QVariant>("16bit signed",   CV_16S);
    colorMaps << QPair<QString, QVariant>("16bit float",    CV_16F);
    colorMaps << QPair<QString, QVariant>("32bit signed",   CV_32S);
    colorMaps << QPair<QString, QVariant>("32bit float",    CV_32F);
    colorMaps << QPair<QString, QVariant>("64bit float",    CV_64F);
    addEnumerationInput("output type", colorMaps);

    Properties alphaProperties;
    alphaProperties.insert("singleStep", 0.1);
    alphaProperties.insert("minimum", -CvConstants::defaultDoubleMax);
    addInput("alpha", PlugType::Double, 1.0, alphaProperties);

    Properties betaProperties;
    betaProperties.insert("minimum", -CvConstants::defaultDoubleMax);
    addInput("beta", PlugType::Double, 0.0, betaProperties);

    // Outputs
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);

    // Help
    addHelpMessage("convertTo",
                   CvUtils::makeUrl({"d3", "d63", "classcv_1_1Mat"}, "a3f356665bb0ca452e7d7723ccac9a810"),
                   HelpMessageType::Function);
}

Properties ConvertToProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat outputImage;
    inputImage.convertTo(outputImage,
                         inputs["output type"].toInt(),
                         inputs["alpha"].toDouble(),
                         inputs["beta"].toDouble());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}

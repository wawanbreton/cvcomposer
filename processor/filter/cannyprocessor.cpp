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

#include "cannyprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"


CannyProcessor::CannyProcessor()
{
    addInput("input image", PlugType::Image, ProcessorListType::Simple);

    addInput("threshold 1", PlugType::Double, 64.0);
    addInput("threshold 2", PlugType::Double, 128.0);

    Properties apertureProperties;
    apertureProperties.insert("decimals", 0);
    apertureProperties.insert("minimum", 1);
    addInput("aperture size", PlugType::Double, 3, apertureProperties);

    addInput("L2gradient", PlugType::Boolean);

    addOutput("output image", PlugType::Image, ProcessorListType::Simple);
}

Properties CannyProcessor::processImpl(const Properties &inputs)
{
    cv::Mat outputImage;
    cv::Canny(inputs["input image"].value<cv::Mat>(),
              outputImage,
              inputs["threshold 1"].toDouble(),
              inputs["threshold 2"].toDouble(),
              inputs["aperture size"].toInt(),
              inputs["L2gradient"].toBool());

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;
}


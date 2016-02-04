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

#include "blurprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "cvutils.h"


BlurProcessor::BlurProcessor() :
    AbstractProcessor()
{
    addInput("input image", PlugType::Image);
    addInput("size",        PlugType::Size);
    addInput("anchor",      PlugType::Point);
    addInput("border",      PlugType::Enumeration);

    addOutput("output image", PlugType::Image);
}

Properties BlurProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat blurred = inputImage.clone();
    cv::blur(inputImage,
             blurred,
             getProperty("size").value<cv::Size>(),
             getProperty("anchor").value<cv::Point>(),
             getProperty("border").toInt());

    Properties properties;
    properties.insert("output image", QVariant::fromValue(blurred));
    return properties;
}


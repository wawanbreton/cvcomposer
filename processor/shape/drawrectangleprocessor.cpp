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

#include "drawrectangleprocessor.h"

#include "global/cvutils.h"


DrawRectangleProcessor::DrawRectangleProcessor()
{
    addInput("input image", PlugType::Image);
    addInput("rectangle", PlugType::Rectangle);
    addInput("color", PlugType::Color, QVariant::fromValue(cv::Scalar(255, 255, 255, 255)));

    addOutput("output image", PlugType::Image);
}

Properties DrawRectangleProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Rect rectangle = inputs["rectangle"].value<cv::Rect>();
    cv::Scalar color = inputs["color"].value<cv::Scalar>();

    cv::Mat outputImage = inputImage.clone();
    cv::rectangle(outputImage, rectangle, color);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(outputImage));
    return outputs;

}

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

#include "averageprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

#include "global/cvutils.h"


AverageProcessor::AverageProcessor()
{
    addInput("input images", PlugType::Image, QVariant(), Properties(), ThreeStateBool::None, true);

    addOutput("output image", PlugType::Image);
}

Properties AverageProcessor::processImpl(const Properties &inputs)
{
    cv::Mat average;
    int count = 0;
    int baseFormat = -1;
    int workFormat = -1;

    foreach(const QVariant &input, inputs["input images"].value<QList<QVariant>>())
    {
        cv::Mat image = input.value<cv::Mat>();

        if(baseFormat < 0)
        {
            baseFormat = CV_MAT_TYPE(image.flags);

            switch(CV_MAT_TYPE(image.flags))
            {
                case CV_8UC1:
                    workFormat = CV_16UC1;
                    break;
                case CV_8UC3:
                    workFormat = CV_16UC3;
                    break;
                case CV_8UC4:
                    workFormat = CV_16UC4;
                    break;
                default:
                    qCritical() << "Unsupported image format";
                    break;
            }

            image.convertTo(average, workFormat);
        }
        else
        {
            cv::Mat convertedImage;
            image.convertTo(convertedImage, workFormat);
            average += convertedImage;
        }

        count++;
    }

    average /= count;

    cv::Mat finalImage;
    average.convertTo(finalImage, baseFormat);

    Properties outputs;
    outputs.insert("output image", QVariant::fromValue(finalImage));
    return outputs;
}


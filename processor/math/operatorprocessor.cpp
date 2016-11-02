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

#include "operatorprocessor.h"

#include <QDebug>

#include "global/cvutils.h"


OperatorProcessor::OperatorProcessor()
{
    // TODO : use generic types instead of images so that we can pass numeric values
    addInput("input image 1", PlugType::Image);
    addInput("input image 2", PlugType::Image);

    QList<QPair<QString, QVariant> > operators;
    operators << QPair<QString, QVariant>("Add", 1);
    operators << QPair<QString, QVariant>("Substract", 2);
    operators << QPair<QString, QVariant>("Absolute substract", 3);
    operators << QPair<QString, QVariant>("Multiply", 4);
    operators << QPair<QString, QVariant>("Divide", 5);
    addEnumerationInput("operator", operators, 1);

    addOutput("image", PlugType::Image);
}

Properties OperatorProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage1 = inputs["input image 1"].value<cv::Mat>();
    cv::Mat inputImage2 = inputs["input image 2"].value<cv::Mat>();
    int operatorId = inputs["operator"].toInt();
    cv::Mat outputImage;

    switch(operatorId)
    {
        case 1:
            outputImage = inputImage1 + inputImage2;
            break;
        case 2:
            outputImage = inputImage1 - inputImage2;
            break;
        case 3:
            cv::absdiff(inputImage1, inputImage2, outputImage);
            break;
        case 4:
            outputImage = inputImage1 * inputImage2;
            break;
        case 5:
            outputImage = inputImage1 / inputImage2;
            break;
        default:
            qCritical() << "Unknown operator" << operatorId;
            break;
    }

    Properties outputs;
    outputs.insert("image", QVariant::fromValue(outputImage));
    return outputs;
}


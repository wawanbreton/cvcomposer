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

#include "imagefromfileprocessor.h"

#include <opencv2/highgui/highgui.hpp>

#include <QDebug>

#include "global/cvutils.h"


ImageFromFileProcessor::ImageFromFileProcessor() :
    AbstractProcessor()
{
    addInput("path",   PlugType::ImagePath);

    QList<QPair<QString, QVariant> > valuesTypes;
    valuesTypes << QPair<QString, int>("Unchanged", CV_LOAD_IMAGE_UNCHANGED);
    valuesTypes << QPair<QString, int>("Grayscale", CV_LOAD_IMAGE_GRAYSCALE);
    valuesTypes << QPair<QString, int>("Color",     CV_LOAD_IMAGE_COLOR);
    valuesTypes << QPair<QString, int>("Any depth", CV_LOAD_IMAGE_ANYDEPTH);
    valuesTypes << QPair<QString, int>("Any color", CV_LOAD_IMAGE_ANYCOLOR);
    addEnumerationInput("mode", valuesTypes, CV_LOAD_IMAGE_COLOR);

    addOutput("image", PlugType::Image);
}

Properties ImageFromFileProcessor::processImpl(const Properties &inputs)
{
    Q_UNUSED(inputs);

    Properties outputs;
    outputs.insert("image", QVariant::fromValue(cv::imread(inputs["path"].toString().toStdString(),
                                                           inputs["mode"].toInt())));

    return outputs;
}

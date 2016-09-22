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
    Properties pathProperties;
    QList<QPair<QString, QStringList> > formats;
    formats << qMakePair(QString("JPEG"), QStringList() << "jpeg" << "jpg" << "jpe");
    formats << qMakePair(QString("Portable network graphics"), QStringList() << "png");
    formats << qMakePair(QString("TIFF"), QStringList() << "tiff" << "tif");
    formats << qMakePair(QString("JPEG 2000"), QStringList() << "jp2");
    formats << qMakePair(QString("Windows Bitmap"), QStringList() << "bmp");
    formats << qMakePair(QString("OpenEXR"), QStringList() << "exr");
    formats << qMakePair(QString("Sun raster"), QStringList() << "sr" << "ras");
    formats << qMakePair(QString("Portable image formats"), QStringList() << "pbm" << "pgm" << "ppm");
    pathProperties.insert("formats", QVariant::fromValue(formats));
    addInput("path", PlugType::Path, QVariant(), pathProperties);

    addEnumerationInput("mode", CvUtils::makeImageLoadFormatsValues(), CV_LOAD_IMAGE_COLOR);

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

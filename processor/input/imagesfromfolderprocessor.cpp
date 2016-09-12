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

#include "imagesfromfolderprocessor.h"

#include <opencv2/highgui/highgui.hpp>

#include <QDir>
#include <QDebug>

#include "global/cvutils.h"


ImagesFromFolderProcessor::ImagesFromFolderProcessor()
{
    addInput("path", PlugType::FolderPath);

    addEnumerationInput("mode", CvUtils::makeImageLoadFormatsValues(), CV_LOAD_IMAGE_COLOR);

    addOutput("images", PlugType::Image, true);
}

Properties ImagesFromFolderProcessor::processImpl(const Properties &inputs)
{
    QList<QVariant> images;

    QDir dir(inputs["path"].toString());
    foreach(const QFileInfo &file, dir.entryInfoList(QDir::Files | QDir::Readable))
    {
        cv::Mat image = cv::imread(file.absoluteFilePath().toStdString(), inputs["mode"].toInt());
        if(image.rows > 0 && image.cols > 0)
        {
            images << QVariant::fromValue(image);
        }
    }

    Properties outputs;
    outputs.insert("images", QVariant::fromValue(images));
    return outputs;
}


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
    // Inputs
    Properties pathProperties;
    pathProperties.insert("folder", true);
    addInput("path", PlugType::Path, QVariant(), pathProperties);

    addEnumerationInput("mode", CvUtils::makeImageLoadFormatsValues(), cv::IMREAD_COLOR);

    // Outputs
    addOutput("images", PlugType::Image, ProcessorListType::Custom);

    // Help
    addHelpMessage("imread",
                   CvUtils::makeUrl({"d4", "da8", "group__imgcodecs"}, "ga288b8b3da0892bd651fce07b3bbd3a56"),
                   HelpMessageType::Function);
    addHelpMessage("tutorial",
                   CvUtils::makeUrl({"db", "d64", "tutorial_load_save_image"}),
                   HelpMessageType::Tutorial);
}

Properties ImagesFromFolderProcessor::processImpl(const Properties &inputs)
{
    QList<QVariant> images;

    QDir dir(inputs["path"].toString());
    for(const QFileInfo &file : dir.entryInfoList(QDir::Files | QDir::Readable))
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


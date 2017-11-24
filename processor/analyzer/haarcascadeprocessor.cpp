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

#include "haarcascadeprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>

#include "global/cvutils.h"


HaarCascadeProcessor::HaarCascadeProcessor()
{
    addInput("image", PlugType::Image);

    QList<QPair<QString, QVariant>> basicClassifiers;
    QDir classifiersDir = QCoreApplication::applicationDirPath();
    QStringList nameFilters = QStringList() << "haarcascade_*.xml";
    for(const QFileInfo &basicClassifierFile : classifiersDir.entryInfoList(nameFilters))
    {
        basicClassifiers << QPair<QString, QVariant>(basicClassifierFile.baseName().mid(12),
                                                     basicClassifierFile.absoluteFilePath());
    }
    addEnumerationInput("basic classifier", basicClassifiers);

    Properties pathProperties;
    QList<QPair<QString, QStringList> > formats;
    formats << QPair<QString, QStringList>("XML files", QStringList() << "xml");
    pathProperties.insert("formats", QVariant::fromValue(formats));
    addInput("custom classifier", PlugType::Path, QVariant(), pathProperties);

    addOutput("objects", PlugType::Rectangle, ProcessorListType::Custom);
}

HaarCascadeProcessor::~HaarCascadeProcessor()
{
    if(_classifier)
    {
        delete _classifier;
        _classifier = Q_NULLPTR;
    }
}

Properties HaarCascadeProcessor::processImpl(const Properties &inputs)
{
    QString classifierFile;
    QString customClassifier = inputs["custom classifier"].toString();
    if(!customClassifier.isEmpty())
    {
        classifierFile = customClassifier;
    }
    else
    {
        classifierFile = inputs["basic classifier"].toString();
    }

    cv::Mat inputImage = inputs["image"].value<cv::Mat>().clone();
    if(inputImage.channels() > 1)
    {
        cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);
    }

    // Use a QMutexLocker in case of OpenCV exception
    QMutexLocker locker(&accessMutex());

    if(classifierFile != _currentClassifier)
    {
        if(_classifier)
        {
            delete _classifier;
        }

        _classifier = new cv::CascadeClassifier(classifierFile.toStdString());
        _currentClassifier = classifierFile;
    }

    std::vector<cv::Rect> objects;
    _classifier->detectMultiScale(inputImage, objects);

    locker.unlock();

    QList<QVariant> objectsConverted;
    for(const cv::Rect &object : objects)
    {
        objectsConverted << QVariant::fromValue(object);
    }

    Properties outputs;
    outputs.insert("objects", objectsConverted);
    return outputs;
}


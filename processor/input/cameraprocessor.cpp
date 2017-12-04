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

#include "cameraprocessor.h"

#include "global/cvutils.h"


CameraProcessor::CameraProcessor()
{
    // Outputs
    addOutput("image", PlugType::Image);

    // Help
    addHelpMessage("VideoCapture",
                   CvUtils::makeUrl({"d8", "dfe", "classcv_1_1VideoCapture"}),
                   HelpMessageType::Class);
}

CameraProcessor::~CameraProcessor()
{
    if(_camera)
    {
        delete _camera;
        _camera = Q_NULLPTR;
    }
}

bool CameraProcessor::getKeepProcessing() const
{
    return _camera && _camera->isOpened();
}

Properties CameraProcessor::processImpl(const Properties &inputs)
{
    Q_UNUSED(inputs);

    cv::Mat outputImage;

    // Use a QMutexLocker in case of OpenCV exception
    QMutexLocker locker(&accessMutex());

    if(!_camera)
    {
        _camera = new cv::VideoCapture(0);
    }

    if(_camera->isOpened() && _camera->grab())
    {
        _camera->retrieve(outputImage);
    }

    locker.unlock();

    Properties properties;
    properties.insert("image", QVariant::fromValue(outputImage));
    return properties;
}

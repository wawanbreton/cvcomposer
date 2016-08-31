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

cv::VideoCapture *CameraProcessor::_camera = NULL;


CameraProcessor::CameraProcessor()
{
    addOutput("image", PlugType::Image);
}

bool CameraProcessor::getRealTimeProcessing() const
{
    return true;
}

void CameraProcessor::cleanup()
{
    if(_camera)
    {
        delete _camera;
        _camera = NULL;
    }
}

Properties CameraProcessor::processImpl(const Properties &inputs)
{
    Q_UNUSED(inputs);

    cv::Mat outputImage;

    if(_camera == NULL)
    {
        _camera = new cv::VideoCapture(0);
    }

    if(_camera->isOpened() && _camera->grab())
    {
        _camera->retrieve(outputImage);
    }

    Properties properties;
    properties.insert("image", QVariant::fromValue(outputImage));
    return properties;
}

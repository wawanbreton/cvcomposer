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


ImageFromFileProcessor::ImageFromFileProcessor() :
    AbstractProcessor()
{

}

QList<cv::Mat> ImageFromFileProcessor::processImpl(const QList<cv::Mat> &inputs)
{
    Q_UNUSED(inputs);
    return QList<cv::Mat>() << cv::imread(getProperty("path").toString().toStdString(),
                                          CV_LOAD_IMAGE_COLOR);
}

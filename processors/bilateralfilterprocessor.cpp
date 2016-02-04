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

#include "bilateralfilterprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>


BilateralFilterProcessor::BilateralFilterProcessor()
{
}

Properties BilateralFilterProcessor::processImpl(const Properties &inputs)
{
    #warning implement this
    /*cv::Mat filtered = inputs[0].clone();
    cv::bilateralFilter(inputs[0],
                        filtered,
                        getProperty("diameter").toInt(),
                        getProperty("sigmaColor").toDouble(),
                        getProperty("sigmaSpace").toDouble(),
                        getProperty("border").toInt());*/
    return Properties();
}

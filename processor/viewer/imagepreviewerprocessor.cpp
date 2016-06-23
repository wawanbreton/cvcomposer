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

#include "imagepreviewerprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <QPixmap>
#include <QDebug>
#include <QElapsedTimer>

#include "global/cvutils.h"


ImagePreviewerProcessor::ImagePreviewerProcessor()
{
    addInput("input image", PlugType::Image, QVariant(), Properties(), ThreeStateBool::False);

    addOutput("output image", PlugType::ImagePreview);
}

Properties ImagePreviewerProcessor::processImpl(const Properties &inputs)
{
    cv::Mat inputImage = inputs["input image"].value<cv::Mat>();
    cv::Mat smallImage;

    // First resize the image if it is too big, because we only require a preview
    double scale;
    int previewSize = 512;
    if(inputImage.cols > inputImage.rows)
    {
        scale = previewSize / double(inputImage.cols);
    }
    else
    {
        scale = previewSize / double(inputImage.rows);
    }

    if(scale < 1)
    {
        cv::resize(inputImage, smallImage, cv::Size(), scale, scale);
    }
    else
    {
        smallImage = inputImage;
    }

    // Now converts the small image to a QPixmap for display
    QPixmap outputImage = QPixmap::fromImage(CvUtils::toQImage(smallImage));

    Properties outputs;
    outputs.insert("output image", outputImage);
    return outputs;
}


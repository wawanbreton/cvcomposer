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

#include "discretefouriertransformprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "global/cvutils.h"

using namespace cv;


DiscreteFourierTransformProcessor::DiscreteFourierTransformProcessor()
{
    addInput("input image", PlugType::Image, ProcessorListType::Simple);
    addOutput("output image", PlugType::Image, ProcessorListType::Simple);
}

Properties DiscreteFourierTransformProcessor::processImpl(const Properties &inputs)
{
    Mat inputImage = inputs["input image"].value<cv::Mat>();

    // Code from http://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
    Mat padded;
    int optimalRows = getOptimalDFTSize(inputImage.rows);
    int optimalCols = getOptimalDFTSize(inputImage.cols);
    copyMakeBorder(inputImage,
                   padded,
                   0,
                   optimalRows - inputImage.rows,
                   0,
                   optimalCols - inputImage.cols,
                   BORDER_CONSTANT,
                   Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImage;
    merge(planes, 2, complexImage);

    dft(complexImage, complexImage);

    split(complexImage, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magnitudeImage = planes[0];

    magnitudeImage += Scalar::all(1);
    log(magnitudeImage, magnitudeImage);

    magnitudeImage = magnitudeImage(Rect(0, 0, magnitudeImage.cols & -2, magnitudeImage.rows & -2));

    int cx = magnitudeImage.cols / 2;
    int cy = magnitudeImage.rows / 2;

    Mat quadrant0(magnitudeImage, Rect(0, 0, cx, cy));
    Mat quadrant1(magnitudeImage, Rect(cx, 0, cx, cy));
    Mat quadrant2(magnitudeImage, Rect(0, cy, cx, cy));
    Mat quadrant3(magnitudeImage, Rect(cx, cy, cx, cy));

    Mat tmp;
    quadrant0.copyTo(tmp);
    quadrant3.copyTo(quadrant0);
    tmp.copyTo(quadrant3);

    quadrant1.copyTo(tmp);
    quadrant2.copyTo(quadrant1);
    tmp.copyTo(quadrant2);

    normalize(magnitudeImage, magnitudeImage, 0, 1, CV_MINMAX);

    Properties properties;
    properties.insert("output image", QVariant::fromValue(magnitudeImage));
    return properties;
}

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

#include "global/cvutils.h"

#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>


QImage CvUtils::toQImage(const cv::Mat &mat)
{
    QImage image;

    // Activate this code to display the OpenCV images codes values as integers
    #if 0
    qDebug() << CV_8UC1;
    qDebug() << CV_8UC2;
    qDebug() << CV_8UC3;
    qDebug() << CV_8UC4;
    qDebug() << "";

    qDebug() << CV_8SC1;
    qDebug() << CV_8SC2;
    qDebug() << CV_8SC3;
    qDebug() << CV_8SC4;
    qDebug() << "";

    qDebug() << CV_16UC1;
    qDebug() << CV_16UC2;
    qDebug() << CV_16UC3;
    qDebug() << CV_16UC4;
    qDebug() << "";

    qDebug() << CV_16SC1;
    qDebug() << CV_16SC2;
    qDebug() << CV_16SC3;
    qDebug() << CV_16SC4;
    qDebug() << "";

    qDebug() << CV_32SC1;
    qDebug() << CV_32SC2;
    qDebug() << CV_32SC3;
    qDebug() << CV_32SC4;
    qDebug() << "";

    qDebug() << CV_32FC1;
    qDebug() << CV_32FC2;
    qDebug() << CV_32FC3;
    qDebug() << CV_32FC4;
    qDebug() << "";

    qDebug() << CV_64FC1;
    qDebug() << CV_64FC2;
    qDebug() << CV_64FC3;
    qDebug() << CV_64FC4;
    #endif

    if(mat.total() != 0)
    {
        if(mat.type() == CV_8UC1) // 8-bits unsigned, 1 channel (grayscale)
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for(int i = 0 ; i < 256; i++)
            {
                colorTable.push_back(qRgb(i,i,i));
            }

            // Create QImage with same dimensions as input Mat
            image = QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            image.setColorTable(colorTable);
        }
        else if(mat.type() == CV_8UC3) // 8-bits unsigned, 3 channels (BGR)
        {
            image = QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            image = image.rgbSwapped();
        }
        else if(mat.type() == CV_32FC1) // 32-bits float, 1 channel (grayscale)
        {
            image = QImage(mat.cols, mat.rows, QImage::Format_RGB32);
            int rows = mat.rows;
            int cols = mat.cols;

            if(mat.isContinuous())
            {
                int total = cols * rows;
                float *dataPointer = (float *)mat.data;

                for(int index = 0 ; index < total ; index++)
                {
                    int row = index / cols;
                    int col = index % cols;
                    quint8 valueChar = dataPointer[index] * 255;
                    image.setPixel(col, row, qRgb(valueChar, valueChar, valueChar));
                }
            }
            else
            {
                const float* rowPointer;
                for(int row = 0; row < rows; row++)
                {
                    rowPointer = mat.ptr<float>(row);
                    for(int col = 0; col < cols; col++)
                    {
                        quint8 valueChar = rowPointer[col] * 255;
                        image.setPixel(col, row, qRgb(valueChar, valueChar, valueChar));
                    }
                }
            }
        }
        else
        {
            qWarning() << "CvUtils::toQImage Unsupported mat type"
                       << mat.type() <<", you will have to work !";
        }
    }

    return image;
}

QList<QPair<QString, QVariant> > CvUtils::makeBlurBorderValues()
{
    QList<QPair<QString, QVariant> > borderValues;
    borderValues << QPair<QString, QVariant>("Reflect 101", cv::BORDER_REFLECT101);
    borderValues << QPair<QString, QVariant>("Reflect",     cv::BORDER_REFLECT);
    borderValues << QPair<QString, QVariant>("Replicate",   cv::BORDER_REPLICATE);
    borderValues << QPair<QString, QVariant>("Constant",    cv::BORDER_CONSTANT);

    return borderValues;
}

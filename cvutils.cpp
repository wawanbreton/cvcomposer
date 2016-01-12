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

#include "cvutils.h"

#include <QDebug>


QImage CvUtils::toQImage(const cv::Mat &mat)
{
    QImage image;

    if(mat.type() == CV_8UC1) // 8-bits unsigned, NO. OF CHANNELS=1
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
    if(mat.type() == CV_8UC3) // 8-bits unsigned, NO. OF CHANNELS=3
    {
        image = QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        image = image.rgbSwapped();
    }
    else
    {
        qWarning() << "CvUtils::toQImage Unsupported mat type, you will have to work !";
    }

    return image;
}

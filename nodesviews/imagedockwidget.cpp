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

#include "imagedockwidget.h"
#include "ui_imagedockwidget.h"

#include <QDebug>


ImageDockWidget::ImageDockWidget(QWidget *parent) :
    QDockWidget(parent),
    _ui(new Ui::ImageDockWidget)
{
    _ui->setupUi(this);
}

ImageDockWidget::~ImageDockWidget()
{
    delete _ui;
}

void ImageDockWidget::setImage(const cv::Mat &image)
{
    QImage qImage;

    #warning factorize this
    if(image.type() == CV_8UC1) // 8-bits unsigned, NO. OF CHANNELS=1
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for(int i = 0 ; i < 256; i++)
        {
            colorTable.push_back(qRgb(i,i,i));
        }

        // Create QImage with same dimensions as input Mat
        qImage = QImage((const uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_Indexed8);
        qImage.setColorTable(colorTable);
    }
    if(image.type() == CV_8UC3) // 8-bits unsigned, NO. OF CHANNELS=3
    {
        qImage = QImage((const uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        qImage = qImage.rgbSwapped();
    }
    else
    {
        qWarning() << "Mat could not be converted to QImage.";
    }

    if(not qImage.isNull())
    {
        _ui->label->setPixmap(QPixmap::fromImage(qImage));
    }
}

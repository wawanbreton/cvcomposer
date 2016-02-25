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

#include "imagepreviewwidget.h"

#include <QPainter>
#include <QVariant>

#include "global/cvutils.h"


ImagePreviewWidget::ImagePreviewWidget(QWidget *parent) :
    AbstractPlugWidget(parent)
{
    setMinimumSize(128, 128);
}

void ImagePreviewWidget::onNodeProcessed(const Properties &inputs, const Properties &outputs)
{
    AbstractPlugWidget::onNodeProcessed(inputs, outputs);

    _image = QPixmap::fromImage(CvUtils::toQImage(inputs["image"].value<cv::Mat>()));
    update();
}

void ImagePreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if(_image.isNull())
    {
        painter.fillRect(rect(), Qt::gray);
    }
    else
    {
        qreal imageRatio = qreal(_image.width()) / _image.height();
        qreal widgetRatio = qreal(width()) / height();
        QRect rect;
        if(imageRatio > widgetRatio)
        {
            rect = QRect(0, 0, width(), width() / imageRatio);
            rect.translate(0, (height() - rect.height()) / 2);
        }
        else
        {
            rect = QRect(0, 0, height() * imageRatio, height());
            rect.translate((width() - rect.width()) / 2, 0);
        }
        painter.drawPixmap(rect, _image);
    }
}

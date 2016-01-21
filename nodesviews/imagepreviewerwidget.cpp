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

#include "imagepreviewerwidget.h"
#include "ui_imagepreviewerwidget.h"

#include <QDebug>
#include <QPainter>

#include "cvutils.h"


ImagePreviewerWidget::ImagePreviewerWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _image()
{
}

ImagePreviewerWidget::ImagePreviewerWidget(const ImagePreviewerWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("ImagePreviewerWidget::ImagePreviewerWidget");
}

QVariant ImagePreviewerWidget::getProperty(const QString &name) const
{
    Q_UNUSED(name);
    return QVariant();
}

QSize ImagePreviewerWidget::sizeHint() const
{
    return QSize(128, 128);
}

void ImagePreviewerWidget::setProperty(const QString &name, const QVariant &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
}

QStringList ImagePreviewerWidget::getPropertiesNames() const
{
    return QStringList();
}

void ImagePreviewerWidget::paintEvent(QPaintEvent *event)
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

void ImagePreviewerWidget::onProcessDone(const QList<cv::Mat> &outputs,
                                         const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Previewer has no ouput, it only displays the input image
    _image = QPixmap::fromImage(CvUtils::toQImage(inputs[0]));
    update();
}

void ImagePreviewerWidget::onProcessUnavailable()
{
    _image = QPixmap();
    update();
}

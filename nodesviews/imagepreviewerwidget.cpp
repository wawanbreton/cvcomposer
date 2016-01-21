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

#include "cvutils.h"


ImagePreviewerWidget::ImagePreviewerWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::ImagePreviewerWidget)
{
    #warning TODO : keep image ratio
    _ui->setupUi(this);
}

ImagePreviewerWidget::ImagePreviewerWidget(const ImagePreviewerWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("ImagePreviewerWidget::ImagePreviewerWidget");
}

ImagePreviewerWidget::~ImagePreviewerWidget()
{
    delete _ui;
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

void ImagePreviewerWidget::onProcessDone(const QList<cv::Mat> &outputs,
                                         const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Previewer has no ouput, it only displays the input image
    _ui->label->setPixmap(QPixmap::fromImage(CvUtils::toQImage(inputs[0])));
}

void ImagePreviewerWidget::onProcessUnavailable()
{
    _ui->label->setPixmap(QPixmap());
}

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

#include "imageviewerdockwidget.h"
#include "ui_imageviewerdockwidget.h"

#include <QDebug>

#include "global/cvutils.h"


int ImageViewerDockWidget::_count = 0;

ImageViewerDockWidget::ImageViewerDockWidget(QWidget *parent) :
    QDockWidget(parent),
    _ui(new Ui::ImageViewerDockWidget)
{
    _ui->setupUi(this);

    // Saving and restoring dock widgets properly requires all of them to have a unique object name
    setObjectName(QString("ImageViewerDockWidget%1").arg(_count++));
}

ImageViewerDockWidget::~ImageViewerDockWidget()
{
    delete _ui;
}

void ImageViewerDockWidget::setImage(const QPixmap &image)
{
    _ui->graphicsView->setImage(image);
}

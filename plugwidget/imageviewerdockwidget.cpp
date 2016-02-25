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
#include "ui_imagedockwidget.h"

#include <QDebug>

#include "global/cvutils.h"


ImageViewerDockWidget::ImageViewerDockWidget(QWidget *parent) :
    QDockWidget(parent),
    _ui(new Ui::ImageDockWidget)
{
    _ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    _ui->graphicsView->setScene(scene);

    _pixmapItem = new QGraphicsPixmapItem();
    scene->addItem(_pixmapItem);
}

ImageViewerDockWidget::~ImageViewerDockWidget()
{
    delete _ui;
}

void ImageViewerDockWidget::setImage(const cv::Mat &image)
{
    QImage qImage = CvUtils::toQImage(image);
    _pixmapItem->setPixmap(QPixmap::fromImage(qImage));
    _ui->graphicsView->setSceneRect(0, 0, qImage.width(), qImage.height());
}

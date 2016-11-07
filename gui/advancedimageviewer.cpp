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

#include "advancedimageviewer.h"

#include <QDebug>
#include <QMouseEvent>


AdvancedImageViewer::AdvancedImageViewer(QWidget *parent) :
    InteractiveGraphicsView(parent),
    _pixmapItem(new QGraphicsPixmapItem())
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(_pixmapItem);
    setScene(scene);

    setMinZoom(-2);
    setMaxZoom(20);
}

void AdvancedImageViewer::setImage(const QPixmap &image)
{
    _pixmapItem->setPixmap(image);
    setSceneRect(0, 0, image.width(), image.height());
}

QPixmap AdvancedImageViewer::getImage() const
{
    return _pixmapItem->pixmap();
}

void AdvancedImageViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        pickColor(event);
    }
    else
    {
        InteractiveGraphicsView::mousePressEvent(event);
    }
}

void AdvancedImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton))
    {
        pickColor(event);
    }
    else
    {
        InteractiveGraphicsView::mouseMoveEvent(event);
    }
}

void AdvancedImageViewer::pickColor(QMouseEvent *event)
{
    event->accept();
    QPointF scenePos = mapToScene(event->pos());
    QPoint pixel(scenePos.x(), scenePos.y());
    emit colorPicked(_pixmapItem->pixmap().toImage().pixel(pixel));
}

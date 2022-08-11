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

#include "interactivegraphicsview.h"

#include <QDebug>
#include <QtMath>
#include <QWheelEvent>
#include <QGraphicsProxyWidget>


InteractiveGraphicsView::InteractiveGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    _zoom(0),
    _minZoom(1),
    _maxZoom(10)
{
    resetZoom();
}

void InteractiveGraphicsView::zoomIn()
{
    zoom(_zoom + 1);
}

void InteractiveGraphicsView::zoomOut()
{
    zoom(_zoom - 1);
}

void InteractiveGraphicsView::resetZoom()
{
    zoom(5);
}

void InteractiveGraphicsView::setMinZoom(int minZoom)
{
    _minZoom = minZoom;
}

void InteractiveGraphicsView::setMaxZoom(int maxZoom)
{
    _maxZoom = maxZoom;
}

void InteractiveGraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsItem *item = itemAt(event->position().toPoint());
    if(item && item->type() == QGraphicsProxyWidget::Type)
    {
        QGraphicsView::wheelEvent(event);
    }
    else
    {
        zoom(_zoom + event->angleDelta().y() / 120);
    }
}

void InteractiveGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
    {
        // We want to drag the scene with the middle button, and Qt implements it with the left
        // button, so make it believe the left button has been pressed;
        setDragMode(QGraphicsView::ScrollHandDrag);
        QMouseEvent pseudoEvent(QMouseEvent::MouseButtonPress,
                                event->pos(),
                                Qt::LeftButton,
                                event->buttons(),
                                event->modifiers());
        QGraphicsView::mousePressEvent(&pseudoEvent);
        viewport()->setCursor(Qt::ClosedHandCursor);
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void InteractiveGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
    {
        QMouseEvent pseudoEvent(QMouseEvent::MouseButtonRelease,
                                event->pos(),
                                Qt::LeftButton,
                                event->buttons(),
                                event->modifiers());
        QGraphicsView::mouseReleaseEvent(&pseudoEvent);
        viewport()->setCursor(Qt::ArrowCursor);
        setDragMode(QGraphicsView::NoDrag);
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void InteractiveGraphicsView::zoom(int scale)
{
    scale = qMin(qMax(scale, _minZoom), _maxZoom);

    if(scale != _zoom)
    {
        _zoom = scale;
        updateTransform();
    }
}

void InteractiveGraphicsView::updateTransform()
{
    resetTransform();
    qreal scaleValue = qExp(((_zoom / 5.0) - 1) * 1.6);
    scale(scaleValue, scaleValue);
}

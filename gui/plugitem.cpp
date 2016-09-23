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

#include "plugitem.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QDebug>

#include "gui/customitems.h"


PlugItem::PlugItem(Plug *plug, QGraphicsItem *parent) :
    QObject(),
    QGraphicsEllipseItem(parent),
    _plug(plug)
{
    setRect(-radius, -radius, radius * 2, radius * 2);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setPen(Qt::NoPen);

    QBrush brush;
    switch(plug->getDefinition().type)
    {
        case PlugType::Generic:
        {
            // Make a rainbow gradient :)
            int precision = 10;
            QLinearGradient gradient;
            gradient.setStart(rect().topLeft() + QPoint(pen().width(), 0));
            gradient.setFinalStop(rect().topRight() - QPoint(pen().width(), 0));
            for(int i = 0 ; i < precision ; i++)
            {
                qreal coeff = qreal(i) / precision;
                gradient.setColorAt(coeff, QColor::fromHsvF(coeff, 0.78, 1));
            }
            brush = gradient;
            break;
        }
        case PlugType::Image:
            brush = QColor(46, 204, 113);
            break;
        case PlugType::Kernel:
            brush = QColor(52, 152, 219);
            break;
        case PlugType::Rectangle:
            brush = QColor(142, 68, 173);
            break;
        case PlugType::Double:
            brush = QColor(230, 126, 34);
            break;
        case PlugType::Circle:
            brush = QColor(85, 110, 134);
            break;
        case PlugType::Contour:
            brush = QColor(241, 196, 15);
            break;
        case PlugType::Line:
            brush = QColor(156, 80, 6);
            break;
        default:
            brush = Qt::white;
            break;
    }

    setBrush(brush);
    setPen(Qt::NoPen);
}

int PlugItem::type() const
{
    return CustomItems::Plug;
}

Plug *PlugItem::getPlug() const
{
    return _plug;
}

void PlugItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);

    if(_plug->getDefinition().supportsList)
    {
        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidthF(2.5);
        pen.setCapStyle(Qt::RoundCap);
        painter->setPen(pen);

        qreal semiWidth = radius * 0.65;
        for(int i = 0 ; i < 3 ; i++)
        {
            painter->drawLine(0, semiWidth, 0, -semiWidth);
            painter->rotate(120);
        }
    }
}

QVariant PlugItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemScenePositionHasChanged)
    {
        emit positionChanged();
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

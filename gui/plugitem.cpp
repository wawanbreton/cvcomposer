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

    if(PlugType::isSingleType(_plug->getDefinition().types))
    {
        setBrush(PlugType::getColor(PlugType::flagsToEnum(plug->getDefinition().types)));
    }
    //QBrush brush;
    /*if(plug->getDefinition().type == PlugType::Generic)
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
    }
    else
    {
        brush = PlugType::getColor(plug->getDefinition().type);
    }*/
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
    if(PlugType::isSingleType(_plug->getDefinition().types))
    {
        return QGraphicsEllipseItem::paint(painter, option, widget);
    }
    else
    {
        //int nbTypes =
    }

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

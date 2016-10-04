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
    _plug(plug),
    _currentAngle(0)
{
    setRect(-radius, -radius, radius * 2, radius * 2);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);

    setCurrentType(PlugType::Image, true);
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
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::NoPen);

    painter->save();
    painter->rotate(_currentAngle);

    QList<PlugType::Enum> types = PlugType::toList(_plug->getDefinition().types);
    qreal delta = 360 / types.count();
    int count = 0;
    foreach(const PlugType::Enum type, types)
    {
        painter->setBrush(PlugType::getColor(type));
        painter->drawPie(rect(), count * delta * 16, delta * 16);
        count++;
    }

    painter->restore();

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

void PlugItem::setCurrentType(PlugType::Enum type, bool input)
{
    QList<PlugType::Enum> types = PlugType::toList(_plug->getDefinition().types);
    int index = types.indexOf(type);
    qreal delta = 360 / types.count();
    qreal partAngle = delta * (index + 0.5);
    qreal target = input ? 180 : 0;

    _currentAngle = partAngle - target;
    update();
}

QVariant PlugItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemScenePositionHasChanged)
    {
        emit positionChanged();
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

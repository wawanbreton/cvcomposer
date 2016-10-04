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

#include "connectionitem.h"

#include <QPainter>

#include "gui/plugitem.h"


ConnectionItem::ConnectionItem(QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    _connection(NULL),
    _itemConnectorOutput(new QGraphicsPathItem(this)),
    _itemConnectorInput(new QGraphicsPathItem(this)),
    _itemLine(new QGraphicsPathItem(this))
{
    _itemConnectorOutput->setBrush(Qt::NoBrush);
    _itemConnectorInput->setBrush(Qt::NoBrush);
    _itemLine->setBrush(Qt::NoBrush);

    setCurrentType(PlugType::Enum(0));

    const qreal connectorRadius = PlugItem::radius + _deltaCenter;
    const qreal connectorAngle = 75;
    QRectF connectorRect(0, 0, connectorRadius * 2, connectorRadius * 2);
    connectorRect.moveCenter(QPointF(0, 0));

    QPainterPath pathOutput;
    pathOutput.arcMoveTo(connectorRect, -connectorAngle);
    pathOutput.arcTo(connectorRect, -connectorAngle, 2 * connectorAngle);
    _itemConnectorOutput->setPath(pathOutput);

    QPainterPath pathInput;
    pathInput.arcMoveTo(connectorRect, 180 - connectorAngle);
    pathInput.arcTo(connectorRect, 180 - connectorAngle, 2 * connectorAngle);
    _itemConnectorInput->setPath(pathInput);
}

QPointF ConnectionItem::getOutput() const
{
    return _itemConnectorOutput->pos();
}

void ConnectionItem::setOutput(const QPointF &output)
{
    _itemConnectorOutput->setPos(output);
    updateLine();
}

void ConnectionItem::setInput(const QPointF &input)
{
    _itemConnectorInput->setPos(input);
    updateLine();
}

void ConnectionItem::setConnection(const Connection *connection)
{
    _connection = connection;
}

const Connection *ConnectionItem::getConnection() const
{
    return _connection;
}

void ConnectionItem::setCurrentType(PlugType::Enum type)
{
    QPen pen;
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(type == PlugType::Enum(0) ? Qt::white : PlugType::getColor(type));

    _itemConnectorOutput->setPen(pen);
    _itemConnectorInput->setPen(pen);
    _itemLine->setPen(pen);
}

void ConnectionItem::updateLine()
{
    const qreal penWidth = _itemLine->pen().widthF();
    const qreal connectorRadius = PlugItem::radius + _deltaCenter;
    const QPointF delta = QPointF(connectorRadius + penWidth / 2, 0);
    const QPointF start = _itemConnectorOutput->pos() + delta;
    const QPointF end = _itemConnectorInput->pos() - delta;
    const qreal lineWidth = end.x() - start.x();
    const QPointF bezierControl = QPointF(qAbs(lineWidth) * 0.5, 0);

    QPainterPath path(start);
    path.cubicTo(start + bezierControl, end - bezierControl, end);
    _itemLine->setPath(path);
}

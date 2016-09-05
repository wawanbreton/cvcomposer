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
    QGraphicsItem(parent),
    _connection(NULL)
{
}

QPointF ConnectionItem::getOutput() const
{
    return _output;
}

void ConnectionItem::setOutput(const QPointF &output)
{
    _output = output;
    update();
}

void ConnectionItem::setInput(const QPointF &input)
{
    _input = input;
    update();
}

void ConnectionItem::setConnection(const Connection *connection)
{
    _connection = connection;
}

const Connection *ConnectionItem::getConnection() const
{
    return _connection;
}

QRectF ConnectionItem::boundingRect() const
{
    return QRectF(_output, _input).adjusted(-100, -100, 100, 100);
}

void ConnectionItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Configure things
    const qreal penWidth = 3;
    const qreal deltaCenter = 3;
    const qreal connectorRadius = PlugItem::radius + deltaCenter;
    const QPointF start = _output + QPointF(connectorRadius + penWidth / 2, 0);
    const QPointF end = _input - QPointF(connectorRadius + penWidth / 2, 0);
    const QPointF bezierControl = QPointF(50, 0);
    const qreal connectorAngle = 75;

    QPen pen(QColor("#2ecc71"), penWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    QPainterPath path;

    // Main line
    path.moveTo(start);
    path.cubicTo(start + bezierControl, end - bezierControl, end);

    // Half-circle on output
    QRectF connectorRect(0, 0, connectorRadius * 2, connectorRadius * 2);
    connectorRect.moveCenter(_input);
    path.arcMoveTo(connectorRect, 180 - connectorAngle);
    path.arcTo(connectorRect, 180 - connectorAngle, 2 * connectorAngle);

    // Half-circle on input
    connectorRect.moveCenter(_output);
    path.arcMoveTo(connectorRect, -connectorAngle);
    path.arcTo(connectorRect, -connectorAngle, 2 * connectorAngle);

    painter->drawPath(path);
}

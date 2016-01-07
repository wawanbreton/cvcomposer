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


ConnectionItem::ConnectionItem(QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    _startPlug(NULL),
    _endPlug(NULL),
    _start(),
    _end()
{
}

const QGraphicsItem *ConnectionItem::getStartPlug() const
{
    return _startPlug;
}

void ConnectionItem::setStartPlug(const QGraphicsItem *startPlug)
{
    _startPlug = startPlug;
    _start = QPointF();
    updateLine();
}

void ConnectionItem::setStart(const QPointF &start)
{
    _start = start;
    _startPlug = NULL;
    updateLine();
}

const QGraphicsItem *ConnectionItem::getEndPlug() const
{
    return _endPlug;
}

void ConnectionItem::setEndPlug(const QGraphicsItem *endPlug)
{
    _endPlug = endPlug;
    _end = QPointF();
    updateLine();
}

void ConnectionItem::setEnd(const QPointF &end)
{
    _end = end;
    _endPlug = NULL;
    updateLine();
}

void ConnectionItem::updateLine()
{
    QLineF line;
    line.setP1(_startPlug ? _startPlug->mapToScene(QPointF(0, 0)) : _start);
    line.setP2(_endPlug ? _endPlug->mapToScene(QPointF(0, 0)) : _end);
    setLine(line);
}

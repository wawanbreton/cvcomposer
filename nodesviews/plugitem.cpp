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

#include <QBrush>
#include <QCursor>


PlugItem::PlugItem(const QUuid &plugId, QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    _plugId(plugId)
{
    setRect(-radius, -radius, radius * 2, radius * 2);
    setBrush(Qt::white);
}

int PlugItem::type() const
{
    #warning Make an enum somewhere
    return QGraphicsItem::UserType + 1;
}

const QUuid &PlugItem::getPlugId() const
{
    return _plugId;
}

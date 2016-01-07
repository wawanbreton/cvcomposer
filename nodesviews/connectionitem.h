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

#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QGraphicsLineItem>

class ConnectionItem : public QGraphicsLineItem
{
    public:
        ConnectionItem(QGraphicsItem *parent = NULL);

        const QGraphicsItem *getStartPlug() const;

        void setStartPlug(const QGraphicsItem *startPlug);

        void setStart(const QPointF &start);

        const QGraphicsItem *getEndPlug() const;

        void setEndPlug(const QGraphicsItem *endPlug);

        void setEnd(const QPointF &end);

    private:
        void updateLine();

    private:
        const QGraphicsItem *_startPlug;
        const QGraphicsItem *_endPlug;
        QPointF _start;
        QPointF _end;
};

#endif // CONNECTIONITEM_H

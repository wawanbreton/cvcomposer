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

#ifndef PLUGITEM_H
#define PLUGITEM_H

#include <QObject>
#include <QGraphicsEllipseItem>

#include "model/plug.h"

class PlugItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

    public:
        PlugItem(Plug *plug, QGraphicsItem *parent = NULL);

        virtual int type() const;

        Plug *getPlug() const;

    signals:
        void positionChanged();

    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    public:
        static const int radius = 5;
        static const int magnetRadius = 20;
        Plug *_plug;
};

#endif // PLUGITEM_H

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

#pragma once

#include <QObject>
#include <QGraphicsEllipseItem>

#include "model/plug.h"

class PlugItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

    public:
        PlugItem(Plug *plug, QGraphicsItem *parent = nullptr);

        virtual int type() const;

        Plug *getPlug() const;

        virtual void paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget = nullptr);

        void setCurrentType(PlugType::Enum type, bool input);

    signals:
        void positionChanged();

    protected:
        virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    private:
        void onCurrentAngleChanged(const QVariant &value);

    public:
        static constexpr int radius = 10;
        static constexpr int magnetRadius = 20;
        static constexpr int maxMultiTypes = 8;

    private:
        Plug *const _plug;
        qreal _currentAngle;
};

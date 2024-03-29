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

#include "gui/interactivegraphicsview.h"

#include <QGraphicsPixmapItem>

class AdvancedImageViewer : public InteractiveGraphicsView
{
    Q_OBJECT

    public:
        explicit AdvancedImageViewer(QWidget *parent = nullptr);

        void setImage(const QPixmap &image);

        QPixmap getImage() const;

    signals:
        void colorPicked(const QColor &color);

    protected:
        virtual void mousePressEvent(QMouseEvent *event) override;

        virtual void mouseMoveEvent(QMouseEvent *event) override;

    private:
        void pickColor(QMouseEvent *event);

    private:
        QGraphicsPixmapItem *const _pixmapItem;
};

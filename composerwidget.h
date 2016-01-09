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

#ifndef COMPOSERWIDGET_H
#define COMPOSERWIDGET_H

#include <QGraphicsView>

#include <QGraphicsScene>

class ComposerWidget : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit ComposerWidget(QWidget *parent = NULL);

        void zoomIn();

        void zoomOut();

        void resetZoom();

    protected:
        virtual void wheelEvent(QWheelEvent *event);

        virtual void mousePressEvent(QMouseEvent *event);

        virtual void mouseReleaseEvent(QMouseEvent *event);

    private:
        void zoom(int scale);

        void updateTransform();

    private:
        QGraphicsScene *_scene;
        int _zoom;
};

#endif // COMPOSERWIDGET_H

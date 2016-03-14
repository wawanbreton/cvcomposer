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

#include "composerwidget.h"

#include "composerscene.h"


ComposerWidget::ComposerWidget(QWidget *parent) :
    InteractiveGraphicsView(parent)
{
    setScene(new ComposerScene(this));
    setSceneRect(-2000, -2000, 4000, 4000);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, true);
}

void ComposerWidget::replaceScene(QGraphicsScene *newScene)
{
    scene()->deleteLater();

    newScene->setParent(this);
    setScene(newScene);
}

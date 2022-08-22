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

#include <QVBoxLayout>

#include "composerscene.h"
#include "model/composermodel.h"
#include "model/node.h"


ComposerWidget::ComposerWidget(QWidget *parent) :
    InteractiveGraphicsView(parent),
    _helpLabel(new QLabel(this))
{
    ComposerScene *scene = new ComposerScene(this);
    setScene(scene);

    setSceneRect(-3000, -3000, 6000, 6000);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, true);
    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // Help label, to disapper as soon as the users has understood what to do
    _helpLabel->setText("Drag & Drop processors here to start composing");
    _helpLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QFont bigFont;
    bigFont.setPointSize(18);
    _helpLabel->setFont(bigFont);
    _helpLabel->setAlignment(Qt::AlignCenter);

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::white);
    _helpLabel->setPalette(palette);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_helpLabel);

    connect(scene->getModel(), &ComposerModel::nodeAdded, _helpLabel, &QLabel::hide);
}

void ComposerWidget::replaceScene(ComposerScene *newScene)
{
    // Let the current scene end its processings before it is destroyed
    ComposerScene *composerScene = qobject_cast<ComposerScene *>(scene());
    if(composerScene)
    {
        connect(composerScene, &ComposerScene::ended, composerScene, &ComposerScene::deleteLater);
    }

    newScene->setParent(this);
    setScene(newScene);

    _helpLabel->hide();
}

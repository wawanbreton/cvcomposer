#include "composerscene.h"

#include <QDebug>
#include <QMimeData>
#include <QGraphicsRectItem>


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void ComposerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    dragMoveEvent(event);
}

void ComposerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();
    }
}

void ComposerScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        QString nodeType = QString::fromUtf8(event->mimeData()->data("application/x-cvcomposerfilter"));
        QGraphicsRectItem *item = new QGraphicsRectItem(QRectF(event->scenePos(), QSizeF(100, 100)));
        addItem(item);
        event->acceptProposedAction();
    }
}

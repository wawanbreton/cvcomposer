#include "composerwidget.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "composerscene.h"


ComposerWidget::ComposerWidget(QWidget *parent) :
    QGraphicsView(parent),
    _scene(new ComposerScene(this))
{
    setScene(_scene);
    setSceneRect(-5000, -5000, 10000, 10000);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
/*
void ComposerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();
    }
}
*/

#include "composerwidget.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>


ComposerWidget::ComposerWidget(QWidget *parent) :
    QGraphicsView(parent)
{
}

void ComposerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();
    }
}

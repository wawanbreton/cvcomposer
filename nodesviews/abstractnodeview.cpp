#include "abstractnodeview.h"

#include <QPainter>


AbstractNodeView::AbstractNodeView(AbstractNode *node, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _node(node)
{
}

QRectF AbstractNodeView::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void AbstractNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawRect(boundingRect());
}

#include "imagepreview.h"


ImagePreview::ImagePreview(AbstractNode *node, QGraphicsItem *parent) :
    QObject(),
    AbstractNodeView(node, parent)
{
}

QRectF ImagePreview::boundingRect() const
{
    QRectF parent = AbstractNodeView::boundingRect();
    parent.setHeight(parent.width());
    return parent;
}

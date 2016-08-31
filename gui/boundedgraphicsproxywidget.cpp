#include "boundedgraphicsproxywidget.h"

#include <QWidget>


BoundedGraphicsProxyWidget::BoundedGraphicsProxyWidget(QGraphicsItem *parent) :
    QGraphicsProxyWidget(parent)
{
}

QRectF BoundedGraphicsProxyWidget::boundingRect() const
{
    if(widget())
    {
        return QRectF(QPointF(0, 0), widget()->sizeHint());
    }

    return QRectF();
}


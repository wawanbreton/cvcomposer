#include "abstractnodeview.h"

#include <QPainter>

#include "nodes/abstractnode.h"


AbstractNodeView::AbstractNodeView(AbstractNode *node, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _node(node)
{
}

QRectF AbstractNodeView::boundingRect() const
{
    return QRectF(-plugRadius, 0, 100 + 2 * plugRadius, 40);
}

void AbstractNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawRect(boundingRect().adjusted(plugRadius, 0, -plugRadius, 0));
    painter->drawText(boundingRect(), Qt::AlignCenter, _node->getUserReadableName());

    painter->setBrush(Qt::white);

    for(quint8 i = 0 ; i < _node->getNbInputs() ; i++)
    {
        painter->drawEllipse(QPointF(0, ((i + 1.0) / (_node->getNbInputs() + 1)) * boundingRect().height()), plugRadius, plugRadius);
    }

    for(quint8 i = 0 ; i < _node->getNbOutputs() ; i++)
    {
        painter->drawEllipse(QPointF(100, ((i + 1.0) / (_node->getNbOutputs() + 1)) * boundingRect().height()), plugRadius, plugRadius);
    }
}

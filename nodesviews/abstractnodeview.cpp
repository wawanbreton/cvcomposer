#include "abstractnodeview.h"

#include <QPainter>
#include <QTimer>

#include "nodes/abstractnode.h"
#include "nodesviews/plugitem.h"


AbstractNodeView::AbstractNodeView(AbstractNode *node, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _node(node),
    _inputPlugs(),
    _outputPlugs()
{
    for(quint8 i = 0 ; i < _node->getNbInputs() ; i++)
    {
        _inputPlugs << new PlugItem(this);
    }

    for(quint8 i = 0 ; i < _node->getNbOutputs() ; i++)
    {
        _outputPlugs << new PlugItem(this);
    }

    QTimer::singleShot(0, this, SLOT(updatePlugs()));
}

const QList<PlugItem *> &AbstractNodeView::getInputs() const
{
    return _inputPlugs;
}

const QList<PlugItem *> &AbstractNodeView::getOutputs() const
{
    return _outputPlugs;
}

QRectF AbstractNodeView::boundingRect() const
{
    return QRectF(0, 0, 100, 30);
}

void AbstractNodeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawRect(boundingRect());
    painter->drawText(boundingRect().adjusted(0, 5, 0, 0), Qt::AlignHCenter | Qt::AlignTop, _node->getUserReadableName());
}

void AbstractNodeView::updatePlugs()
{
    for(quint8 i = 0 ; i < _node->getNbInputs() ; i++)
    {
        _inputPlugs[i]->setPos(QPointF(boundingRect().left(), ((i + 1.0) / (_node->getNbInputs() + 1)) * boundingRect().height()));
    }

    for(quint8 i = 0 ; i < _node->getNbOutputs() ; i++)
    {
        _outputPlugs[i]->setPos(QPointF(boundingRect().right(), ((i + 1.0) / (_node->getNbOutputs() + 1)) * boundingRect().height()));
    }
}

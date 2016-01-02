#ifndef ABSTRACTNODEVIEW_H
#define ABSTRACTNODEVIEW_H

#include <QGraphicsItem>

class AbstractNode;

class AbstractNodeView : public QGraphicsItem
{
    public:
        AbstractNodeView(AbstractNode *node, QGraphicsItem *parent = NULL);

    protected:
        virtual QRectF boundingRect() const;

        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        AbstractNode *_node;
};

#endif // ABSTRACTNODEVIEW_H

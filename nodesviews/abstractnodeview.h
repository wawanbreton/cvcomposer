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
        static const int plugRadius = 5;

    private:
        AbstractNode *_node;
        QWidget *_nodeWidget;
};

#endif // ABSTRACTNODEVIEW_H

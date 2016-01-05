#ifndef ABSTRACTNODEVIEW_H
#define ABSTRACTNODEVIEW_H

#include <QObject>
#include <QGraphicsItem>

#include <QGraphicsEllipseItem>

class AbstractNode;

class AbstractNodeView : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    public:
        AbstractNodeView(AbstractNode *node, QGraphicsItem *parent = NULL);

    protected:
        virtual QRectF boundingRect() const;

        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    protected:
        static const int plugRadius = 5;

    private:
        QGraphicsEllipseItem *addPlug();

    private slots:
        void updatePlugs();

    private:
        AbstractNode *_node;
        QWidget *_nodeWidget;
        QList<QGraphicsEllipseItem *> _inputPlugs;
        QList<QGraphicsEllipseItem *> _outputPlugs;
};

#endif // ABSTRACTNODEVIEW_H

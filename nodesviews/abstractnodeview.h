#ifndef ABSTRACTNODEVIEW_H
#define ABSTRACTNODEVIEW_H

#include <QObject>
#include <QGraphicsItem>

#include <QGraphicsEllipseItem>

class AbstractNode;
class PlugItem;

class AbstractNodeView : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    public:
        AbstractNodeView(AbstractNode *node, QGraphicsItem *parent = NULL);

        const QList<PlugItem *> &getInputs() const;

        const QList<PlugItem *> &getOutputs() const;

    protected:
        virtual QRectF boundingRect() const;

        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private slots:
        void updatePlugs();

    private:
        AbstractNode *_node;
        QWidget *_nodeWidget;
        QList<PlugItem *> _inputPlugs;
        QList<PlugItem *> _outputPlugs;
};

#endif // ABSTRACTNODEVIEW_H

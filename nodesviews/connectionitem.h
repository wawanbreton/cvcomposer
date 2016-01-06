#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QGraphicsLineItem>

class ConnectionItem : public QGraphicsLineItem
{
    public:
        ConnectionItem(QGraphicsItem *parent = NULL);

        const QGraphicsItem *getStartPlug() const;

        void setStartPlug(const QGraphicsItem *startPlug);

        void setStart(const QPointF &start);

        const QGraphicsItem *getEndPlug() const;

        void setEndPlug(const QGraphicsItem *endPlug);

        void setEnd(const QPointF &end);

    private:
        void updateLine();

    private:
        const QGraphicsItem *_startPlug;
        const QGraphicsItem *_endPlug;
        QPointF _start;
        QPointF _end;
};

#endif // CONNECTIONITEM_H

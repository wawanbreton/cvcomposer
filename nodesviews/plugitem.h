#ifndef PLUGITEM_H
#define PLUGITEM_H

#include <QGraphicsEllipseItem>

class PlugItem : public QGraphicsEllipseItem
{
    public:
        PlugItem(QGraphicsItem *parent = NULL);

        virtual int type() const;

    public:
        static const int radius = 5;
};

#endif // PLUGITEM_H

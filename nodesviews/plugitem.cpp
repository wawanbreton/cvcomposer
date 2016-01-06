#include "plugitem.h"

#include <QBrush>
#include <QCursor>


PlugItem::PlugItem(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent)
{
    setRect(-radius, -radius, radius * 2, radius * 2);
    setBrush(Qt::white);
    setCursor(Qt::PointingHandCursor);
}

int PlugItem::type() const
{
    #warning Make an enum somewhere
    return QGraphicsItem::UserType + 1;
}

#include "connectionitem.h"


ConnectionItem::ConnectionItem(QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    _startPlug(NULL),
    _endPlug(NULL),
    _start(),
    _end()
{
}

const QGraphicsItem *ConnectionItem::getStartPlug() const
{
    return _startPlug;
}

void ConnectionItem::setStartPlug(const QGraphicsItem *startPlug)
{
    _startPlug = startPlug;
    _start = QPointF();
    updateLine();
}

void ConnectionItem::setStart(const QPointF &start)
{
    _start = start;
    _startPlug = NULL;
    updateLine();
}

const QGraphicsItem *ConnectionItem::getEndPlug() const
{
    return _endPlug;
}

void ConnectionItem::setEndPlug(const QGraphicsItem *endPlug)
{
    _endPlug = endPlug;
    _end = QPointF();
    updateLine();
}

void ConnectionItem::setEnd(const QPointF &end)
{
    _end = end;
    _endPlug = NULL;
    updateLine();
}

void ConnectionItem::updateLine()
{
    QLineF line;
    line.setP1(_startPlug ? _startPlug->mapToScene(QPointF(0, 0)) : _start);
    line.setP2(_endPlug ? _endPlug->mapToScene(QPointF(0, 0)) : _end);
    setLine(line);
}

// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GENERICNODEITEM_H
#define GENERICNODEITEM_H

#include <QObject>
#include <QGraphicsItem>

#include <QGraphicsEllipseItem>
#include <QWidget>

class AbstractNode;
class PlugItem;

class GenericNodeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    public:
        GenericNodeItem(AbstractNode *node, QGraphicsItem *parent = NULL);

        virtual int type() const;

        const AbstractNode *getNode() const;

        const QList<PlugItem *> &getInputs() const;

        const QList<PlugItem *> &getOutputs() const;

    protected:
        virtual QRectF boundingRect() const;

        virtual void paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget);

        void setWidget(QWidget *widget);

    private slots:
        void updatePlugs();

    private:
        AbstractNode *_node;
        QWidget *_widget;
        QList<PlugItem *> _inputPlugs;
        QList<PlugItem *> _outputPlugs;
};

#endif // GENERICNODEITEM_H
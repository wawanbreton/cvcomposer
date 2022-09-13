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

#pragma once

#include <QObject>
#include <QGraphicsItem>

#include <QAbstractAnimation>
#include <QGraphicsEllipseItem>
#include <QWidget>

class AbstractPlugWidget;
class GenericNodeWidget;
class Node;
class PlugItem;
class Plug;
class Properties;

class GenericNodeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

    public:
        GenericNodeItem(Node *node, QGraphicsItem *parent = nullptr);

        virtual int type() const override;

        const Node *getNode() const;

        Node *accessNode();

        const QList<PlugItem *> &getInputs() const;

        const AbstractPlugWidget *getWidget(const QString &name) const;

        AbstractPlugWidget *accessWidget(const QString &name);

        const QList<PlugItem *> &getOutputs() const;

        void setPlugProperty(const QString &name, const QVariant &value);

        QMap<QString, QString> save() const;

        void load(const QMap<QString, QString> &properties);

        void executionStarted();

        void executionProgress(qreal progress);

        void executionEnded(const Properties &outputs,
                            const Properties &inputs,
                            qint64 duration,
                            const QString &error);

        void nodeInvalid();

        QCursor overrideMouseCursor(const QPointF &mousePos);

        bool startDragging(const QPointF &mousePos);

        virtual QRectF boundingRect() const override;

    signals:
        void plugValueChanged(const QString &name, const QVariant &value);

    protected:
        virtual void paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) override;

        void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        QRectF computeBaseRect() const;

        void onPlugConnectionChanged(const Plug *connectedTo);

        void recomputeSizes();

        void setExecutionMarkOpacity(const QVariant &value);

        void onExecutionAnimationOver();

        void onHelpMenuActionTriggered(QAction *action);

    public:
        static constexpr int titleHeight = 24;
        static constexpr int selectionBorderWidth = 3;
        static constexpr int titleFontSize = 18;
        static constexpr int bottomFontSize = 14;
        static constexpr qreal markSide = 16;
        static constexpr qreal markMargin = (titleHeight - markSide) / 2;
        static constexpr int markExtraSide = 2;

    private:
        Node *const _node;
        GenericNodeWidget *const _widget;
        QGraphicsProxyWidget *const _widgetProxy;
        QList<PlugItem *> _inputPlugs;
        QList<PlugItem *> _outputPlugs;
        QAbstractAnimation *_animationExecution{nullptr};
        qreal _executionMarkOpacity{0.0};
        QString _executionDuration;
        QString _executionError;
        qreal _executionProgress{-1};
        bool _mouseOverBottom{false};
        bool _mouseOverHelp{false};
};

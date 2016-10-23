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

#include "genericnodeitem.h"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTimer>
#include <QLayout>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QVariantAnimation>

#include "model/node.h"
#include "gui/boundedgraphicsproxywidget.h"
#include "gui/customitems.h"
#include "gui/plugitem.h"
#include "gui/genericnodewidget.h"
#include "model/plug.h"


GenericNodeItem::GenericNodeItem(Node *node, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    _node(node),
    _widget(new GenericNodeWidget()),
    _inputPlugs(),
    _outputPlugs(),
    _animationExecution(NULL),
    _executionMarkOpacity(0),
    _executionDuration(),
    _executionError()
{
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    _widget->setPlugs(node->getInputs(), node->getOutputs());
    _widget->setAutoFillBackground(false);
    _widget->setAttribute(Qt::WA_NoBackground, true);

    connect(_widget, SIGNAL(sizeHintChanged()),
                     SLOT(recomputeSizes()));
    connect(_widget, SIGNAL(propertyChanged(QString,QVariant)),
            node,    SLOT(setProperty(QString,QVariant)));
    connect(node,    SIGNAL(processDone(Properties,Properties)),
            _widget, SLOT(onProcessDone(Properties,Properties)));
    connect(node,    SIGNAL(processUnavailable()),
            _widget, SLOT(onProcessUnavailable()));

    QGraphicsProxyWidget *proxy = new BoundedGraphicsProxyWidget(this);
    proxy->setWidget(_widget);
    proxy->setPos(2 * PlugItem::radius, titleHeight + PlugItem::radius);

    foreach(Plug *plug, _node->getInputs())
    {
        if(PlugType::isInputPluggable(plug->getDefinition().type) != PlugType::ManualOnly)
        {
            PlugItem *plugItem = new PlugItem(plug, this);
            _inputPlugs << plugItem;
            connect(plug, SIGNAL(connectionChanged(const Plug*)),
                          SLOT(onPlugConnectionChanged(const Plug*)));
        }
    }
    foreach(Plug *plug, _node->getOutputs())
    {
        if(!PlugType::isOutputInternal(plug->getDefinition().type))
        {
            PlugItem *plugItem = new PlugItem(plug, this);
            _outputPlugs << plugItem;
        }
    }

    recomputeSizes();
}

int GenericNodeItem::type() const
{
    return CustomItems::Node;
}

const Node *GenericNodeItem::getNode() const
{
    return _node;
}

Node *GenericNodeItem::accessNode()
{
    return _node;
}

const QList<PlugItem *> &GenericNodeItem::getInputs() const
{
    return _inputPlugs;
}

const AbstractPlugWidget *GenericNodeItem::getInputWidget(const QString &name) const
{
    return _widget->getInputWidget(name);
}

AbstractPlugWidget *GenericNodeItem::accessInputWidget(const QString &name)
{
    return _widget->accessInputWidget(name);
}

const QList<PlugItem *> &GenericNodeItem::getOutputs() const
{
    return _outputPlugs;
}

void GenericNodeItem::setPlugProperty(const QString &name, const QVariant &value)
{
    _widget->setPlugProperty(name, value);
}

QMap<QString, QString> GenericNodeItem::save() const
{
    QMap<QString, QString> result;

    QString itemPos("%1:%2");
    itemPos = itemPos.arg(QString::number(pos().x(), 'f', 2));
    itemPos = itemPos.arg(QString::number(pos().y(), 'f', 2));

    result.insert("pos", itemPos);

    return result;
}

void GenericNodeItem::load(const QMap<QString, QString> &properties)
{
    QString pos = properties["pos"];
    QStringList posParts = pos.split(':', QString::SkipEmptyParts);
    if(posParts.count() == 2)
    {
        setPos(posParts[0].toDouble(), posParts[1].toDouble());
    }
    else
    {
        qWarning() << "Badly formatted item position :" << pos;
    }
}

void GenericNodeItem::executionStarted()
{
    if(_animationExecution == NULL)
    {
        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::InOutSine);
        animation->setDuration(500);
        connect(animation, SIGNAL(valueChanged(QVariant)), SLOT(setExecutionMarkOpacity(QVariant)));
        connect(animation, SIGNAL(finished()), SLOT(onExecutionAnimationOver()));
        animation->start();

        _animationExecution = animation;
    }
}

void GenericNodeItem::executionEnded(qint64 duration, const QString &error)
{
    if(_animationExecution != NULL)
    {
        delete _animationExecution;
        _animationExecution = NULL;
    }

    _executionError = error;

    if(duration < 1000)
    {
        _executionDuration = QString::number(duration) + " ms";
    }
    else
    {
        _executionDuration = QString::number(duration / 1000.0, 'f', 3) + " s";
    }

    _executionMarkOpacity = 0;
    update();
}

QRectF GenericNodeItem::boundingRect() const
{
    QRectF rect = computeBaseRect();

    qreal semiBorderWidth = selectionBorderWidth / 2.0;
    rect.adjust(-semiBorderWidth, -semiBorderWidth, semiBorderWidth, semiBorderWidth);

    return rect;
}

void GenericNodeItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::NoPen);
    QRectF baseRect = computeBaseRect();
    painter->setBrush(QColor("#efa14c"));
    painter->drawRect(baseRect);

    // Draw the upper title rectangle
    QRectF titleRect = baseRect;
    titleRect.setHeight(titleHeight);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#ea8a1f"));
    painter->drawRect(titleRect);

    // Draw the lower status rectangle
    QRectF bottomRect = titleRect;
    bottomRect.moveBottom(baseRect.bottom());
    painter->drawRect(bottomRect);

    // Draw the outer selection frame
    if(option->state.testFlag(QStyle::State_Selected))
    {
        painter->setBrush(Qt::NoBrush);

        QPen borderPen(Qt::white, selectionBorderWidth);
        borderPen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(borderPen);
        painter->drawRect(baseRect);
    }

    // Draw the title
    QFont font;
    font.setPixelSize(titleFontSize);
    painter->setFont(font);

    if(option->state.testFlag(QStyle::State_Selected))
    {
        painter->setPen(Qt::white);
    }
    else
    {
        painter->setPen(Qt::black);
    }
    painter->drawText(QRect(0, 0, baseRect.width(), titleHeight),
                      Qt::AlignCenter,
                      _node->getUserReadableName());

    // Draw the execution mark
    if(_executionMarkOpacity > 0)
    {
        const qreal markRadius = 8;
        QRectF markRect(0, 0, markRadius * 2, markRadius * 2);
        qreal side = (titleHeight - markRadius * 2) / 2;
        markRect.moveRight(baseRect.width() - side);
        markRect.moveBottom(baseRect.height() - side);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->setOpacity(_executionMarkOpacity);
        painter->drawEllipse(markRect);
        painter->setOpacity(1);
    }

    // Draw the execution duration
    if(!_executionDuration.isEmpty())
    {
        font.setPixelSize(bottomFontSize);
        painter->setFont(font);

        QRectF durationRect = bottomRect;
        durationRect.setLeft(6);
        painter->setPen(widget->palette().text().color());
        painter->drawText(durationRect, Qt::AlignLeft | Qt::AlignVCenter, _executionDuration);
    }
}

QRectF GenericNodeItem::computeBaseRect() const
{
    int widgetWidth = _widget->sizeHint().width() + 4 * PlugItem::radius;
    QFont titleFont;
    titleFont.setPixelSize(titleFontSize);
    QFontMetrics metrics(titleFont);
    int titleWidth = metrics.boundingRect(_node->getUserReadableName()).width() + 2 * PlugItem::radius;

    return QRectF(0,
                  0,
                  qMax(widgetWidth, titleWidth),
                  titleHeight * 2 + _widget->sizeHint().height() + 2 * PlugItem::radius);
}

void GenericNodeItem::onPlugConnectionChanged(const Plug *connectedTo)
{
    Plug *plug = qobject_cast<Plug *>(sender());
    if(plug)
    {
        _widget->setInputPlugged(plug->getDefinition().name, connectedTo != NULL);
    }
    else
    {
        qCritical() << "GenericNodeItem::onPlugConnectionChanged" << "Sender is not a Plug";
    }
}

void GenericNodeItem::recomputeSizes()
{
    QRectF actualBaseRect = computeBaseRect();
    _widget->resize(actualBaseRect.width() - 4 * PlugItem::radius,
                    actualBaseRect.height() - titleHeight * 2 - 4 * PlugItem::radius);

    foreach(PlugItem *plugItem, _inputPlugs)
    {
        plugItem->setPos(QPointF(actualBaseRect.left(),
                                 _widget->y() + _widget->getPlugPosY(plugItem->getPlug()->getDefinition().name)));
    }
    foreach(PlugItem *plugItem, _outputPlugs)
    {
        plugItem->setPos(QPointF(actualBaseRect.right(),
                                 _widget->y() + _widget->getPlugPosY(plugItem->getPlug()->getDefinition().name)));
    }

    prepareGeometryChange();
}

void GenericNodeItem::setExecutionMarkOpacity(const QVariant &value)
{
    _executionMarkOpacity = value.toReal();
    update();
}

void GenericNodeItem::onExecutionAnimationOver()
{
    QAbstractAnimation::Direction direction = _animationExecution->direction();
    _animationExecution->setDirection(direction == QAbstractAnimation::Forward ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);
    _animationExecution->start();
}

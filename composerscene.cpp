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

#include "composerscene.h"

#include <QDebug>
#include <QMimeData>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "composermodel.h"
#include "connection.h"
#include "nodestypesmanager.h"
#include "genericnode.h"
#include "nodesviews/genericnodeitem.h"
#include "nodesviews/customitems.h"
#include "nodesviews/connectionitem.h"
#include "nodesviews/plugitem.h"
#include "nodesviews/abstractnodewidget.h"


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent),
    _model(new ComposerModel(this)),
    _editedConnection(),
    _editedNode(),
    _connections()
{
    _editedConnection.item = NULL;
    _editedConnection.plugInput = NULL;
    _editedConnection.plugOutput = NULL;

    _editedNode.item = NULL;

    connect(_model, SIGNAL(connectionAdded(Connection *)),
                    SLOT(onConnectionAdded(Connection *)));
    connect(_model, SIGNAL(connectionRemoved(Connection *)),
                    SLOT(onConnectionRemoved(Connection *)));
}

void ComposerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    dragMoveEvent(event);
}

void ComposerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();
    }
}

void ComposerScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-cvcomposerfilter"))
    {
        event->acceptProposedAction();

        QString nodeType = QString::fromUtf8(event->mimeData()->data("application/x-cvcomposerfilter"));
        QString nodeName = QString::fromUtf8(event->mimeData()->data("application/x-cvcomposername"));

        GenericNode *node = new GenericNode(nodeType, nodeName);
        _model->addNode(node);

        GenericNodeItem *item = new GenericNodeItem(node);
        QMetaType widgetType(QMetaType::type((nodeType + "Widget").toUtf8()));
        if(widgetType.isValid())
        {
            AbstractNodeWidget *widget = static_cast<AbstractNodeWidget *>(widgetType.create());
            item->setWidget(widget);
            connect(widget, SIGNAL(propertyChanged(QString,QVariant)),
                    node,   SLOT(setProperty(QString,QVariant)));
            connect(node,   SIGNAL(processDone(QList<cv::Mat>,QList<cv::Mat>)),
                    widget, SLOT(onProcessDone(QList<cv::Mat>,QList<cv::Mat>)));
            connect(node,   SIGNAL(processUnavailable()),
                    widget, SLOT(onProcessUnavailable()));
            node->setProperties(widget->getProperties());
        }
        else
        {
            qCritical() << "ComposerScene::dropEvent" << "Unable to find widget for" << nodeType;
        }

        item->setPos(event->scenePos());
        addItem(item);
        _nodes << item;
    }
}

void ComposerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item)
        {
            if(item->type() == QGraphicsProxyWidget::Type)
            {
                // Let the parent do its job fully
                return QGraphicsScene::mousePressEvent(event);
            }
            else if(item->type() == CustomItems::Plug)
            {
                PlugItem *plug = static_cast<PlugItem *>(item);
                bool isInput = _model->findInputPlug(plug->getPlug()) != NULL;
                bool isOutput = _model->findOutputPlug(plug->getPlug()) != NULL;

                _editedConnection.item = new ConnectionItem();
                addItem(_editedConnection.item);

                // When editing an input node, edit its actual connection if there is one
                if(isInput)
                {
                    foreach(ConnectionItem *connectionItem, _connections)
                    {
                        Connection *connection = connectionItem->getConnection();
                        if(connection->getInput() == plug->getPlug())
                        {
                            // We have found an existing connection, edit it
                            _editedConnection.item->setOutput(connectionItem->getOutput());
                            _editedConnection.item->setInput(event->scenePos());
                            _editedConnection.plugOutput = connection->getOutput();
                            _editedConnection.fromOutput = true;
                            _model->removeConnection(connectionItem->getConnection());
                            return;
                        }
                    }
                }

                // We are not editing an existing connection
                if(isInput)
                {
                    _editedConnection.item->setOutput(event->scenePos());
                    _editedConnection.item->setInput(plug->mapToScene(QPointF(0, 0)));
                    _editedConnection.plugInput = plug->getPlug();
                    _editedConnection.fromOutput = false;
                }
                else if(isOutput)
                {
                    _editedConnection.item->setOutput(plug->mapToScene(QPointF(0, 0)));
                    _editedConnection.item->setInput(event->scenePos());
                    _editedConnection.plugOutput = plug->getPlug();
                    _editedConnection.fromOutput = true;
                }
                else
                {
                    qCritical("Selected plug not found in node inputs/outputs");
                }
            }
            else if(item->type() == CustomItems::Node)
            {
                event->widget()->setCursor(Qt::ClosedHandCursor);
                _editedNode.item = static_cast<GenericNodeItem *>(item);
                _editedNode.initClickPos = event->scenePos();
                _editedNode.initNodePose = _editedNode.item->pos();
            }
        }
    }
}

void ComposerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QCursor cursor = Qt::ArrowCursor;

    if(_editedConnection.item)
    {
        bool plugFound = false;

        foreach(QGraphicsItem *itemUnderMouse, items(event->scenePos()))
        {
            if(itemUnderMouse != _editedConnection.item &&
               itemUnderMouse->type() == CustomItems::Plug)
            {
                PlugItem *plugUnderMouse = static_cast<PlugItem *>(itemUnderMouse);

                if(_editedConnection.fromOutput)
                {
                    if(_model->findInputPlug(plugUnderMouse->getPlug()))
                    {
                        _editedConnection.item->setInput(plugUnderMouse->mapToScene(QPointF(0, 0)));
                        _editedConnection.plugInput = plugUnderMouse->getPlug();
                        cursor = Qt::PointingHandCursor;
                        plugFound = true;
                    }
                }
                else
                {
                    if(_model->findOutputPlug(plugUnderMouse->getPlug()))
                    {
                        _editedConnection.item->setOutput(plugUnderMouse->mapToScene(QPointF(0, 0)));
                        _editedConnection.plugOutput = plugUnderMouse->getPlug();
                        cursor = Qt::PointingHandCursor;
                        plugFound = true;
                    }
                }

                break;
            }
        }

        if(not plugFound)
        {
            if(_editedConnection.fromOutput)
            {
                _editedConnection.item->setInput(event->scenePos());
                _editedConnection.plugInput = NULL;
            }
            else
            {
                _editedConnection.item->setOutput(event->scenePos());
                _editedConnection.plugOutput = NULL;
            }
        }
    }
    else if(_editedNode.item)
    {
        cursor = Qt::ClosedHandCursor;

        // Move node, easy part
        _editedNode.item->setPos(_editedNode.initNodePose + (event->scenePos() - _editedNode.initClickPos));

        // Move connections, hard part
        foreach(ConnectionItem *connectionItem, _connections)
        {
            Connection *connection = connectionItem->getConnection();

            if(_editedNode.item->getNode()->hasInput(connection->getInput()))
            {
                foreach(PlugItem *plugItem, _editedNode.item->getInputs())
                {
                    if(plugItem->getPlug() == connection->getInput())
                    {
                        connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                        break;
                    }
                }
            }

            if(_editedNode.item->getNode()->hasOutput(connection->getOutput()))
            {
                foreach(PlugItem *plugItem, _editedNode.item->getOutputs())
                {
                    if(plugItem->getPlug() == connection->getOutput())
                    {
                        connectionItem->setOutput(plugItem->mapToScene(QPointF(0, 0)));
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // We are not editing anything
        QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
        if(item)
        {
            if(item->type() == QGraphicsProxyWidget::Type)
            {
                // Let the parent do its job fully
                return QGraphicsScene::mouseMoveEvent(event);
            }
            else if(item->type() == CustomItems::Plug)
            {
                cursor = Qt::PointingHandCursor;
            }
            else if(item->type() == CustomItems::Node)
            {
                cursor = Qt::OpenHandCursor;
            }
        }
    }

    event->widget()->setCursor(cursor);
}

void ComposerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if(_editedConnection.item)
    {
        if(_editedConnection.plugInput && _editedConnection.plugOutput)
        {
            _model->addConnection(_editedConnection.plugOutput, _editedConnection.plugInput);
        }

        removeItem(_editedConnection.item);
        delete _editedConnection.item;

        _editedConnection.item = NULL;
        _editedConnection.plugInput = NULL;
        _editedConnection.plugOutput = NULL;
    }
    else if(_editedNode.item)
    {
        _editedNode.item = NULL;
        event->widget()->setCursor(Qt::OpenHandCursor);
    }
    else
    {
        // Let the parent do its job fully
        return QGraphicsScene::mouseReleaseEvent(event);
    }
}

void ComposerScene::onConnectionAdded(Connection *connection)
{
    ConnectionItem *connectionItem = new ConnectionItem();
    connectionItem->setConnection(connection);

    foreach(const GenericNodeItem *nodeView, _nodes)
    {
        foreach(const PlugItem *plugItem, nodeView->getInputs())
        {
            if(plugItem->getPlug() == connection->getInput())
            {
                connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
        foreach(const PlugItem *plugItem, nodeView->getOutputs())
        {
            if(plugItem->getPlug() == connection->getOutput())
            {
                connectionItem->setOutput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
    }

    addItem(connectionItem);
    _connections << connectionItem;
}

void ComposerScene::onConnectionRemoved(Connection *connection)
{
    foreach(ConnectionItem *connectionItem, _connections)
    {
        if(connectionItem->getConnection() == connection)
        {
            removeItem(connectionItem);
            delete connectionItem;
            _connections.removeAll(connectionItem);
            break;
        }
    }
}

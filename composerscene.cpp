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

#include "composermodel.h"
#include "nodestypesmanager.h"
#include "nodes/abstractnode.h"
#include "nodes/imagepreviewernode.h"
#include "nodesviews/abstractnodeview.h"
#include "nodesviews/customitems.h"
#include "nodesviews/connectionitem.h"
#include "nodesviews/imagepreview.h"
#include "nodesviews/plugitem.h"


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent),
    _model(new ComposerModel(this)),
    _editedConnection(),
    _editedNode(),
    _connections()
{
    _editedConnection.item = NULL;
    _editedNode.item = NULL;

    connect(_model, SIGNAL(connectionAdded(QUuid)), SLOT(onConnectionAdded(QUuid)));
    connect(_model, SIGNAL(connectionRemoved(QUuid)), SLOT(onConnectionRemoved(QUuid)));
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
        AbstractNode *node = NodesTypesManager::createNode(nodeType);
        if(node)
        {
            _model->addNode(node);
            AbstractNodeView *nodeView;

            if(qobject_cast<ImagePreviewerNode *>(node))
            {
                nodeView = new ImagePreview(node);
            }
            else
            {
                nodeView = new AbstractNodeView(node);
            }

            nodeView->setPos(event->scenePos());
            addItem(nodeView);
            _nodes << nodeView;
        }
    }
}

void ComposerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->widget()->setMouseTracking(true);

    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if(item)
    {
        if(item->type() == CustomItems::Plug)
        {
            PlugItem *plug = static_cast<PlugItem *>(item);
            bool isInput = _model->findInputPlug(plug->getPlugId()) != NULL;
            bool isOutput = _model->findOutputPlug(plug->getPlugId()) != NULL;

            _editedConnection.item = new ConnectionItem();
            addItem(_editedConnection.item);

            // When editing an input node, edit its actual connection if there is one
            if(isInput)
            {
                foreach(ConnectionItem *connectionItem, _connections)
                {
                    Connection connection = _model->getConnection(connectionItem->getConnectionId());
                    if(connection.input == plug->getPlugId())
                    {
                        // We have found an existing connection, edit it
                        _editedConnection.item->setOutput(connectionItem->getOutput());
                        _editedConnection.item->setInput(event->scenePos());
                        _editedConnection.plugOutputId = connection.output;
                        _editedConnection.fromOutput = true;
                        _model->removeConnection(connectionItem->getConnectionId());
                        return;
                    }
                }
            }

            // We are not editing an existing connection
            if(isInput)
            {
                _editedConnection.item->setOutput(event->scenePos());
                _editedConnection.item->setInput(plug->mapToScene(QPointF(0, 0)));
                _editedConnection.plugInputId = plug->getPlugId();
                _editedConnection.fromOutput = false;
            }
            else if(isOutput)
            {
                _editedConnection.item->setOutput(plug->mapToScene(QPointF(0, 0)));
                _editedConnection.item->setInput(event->scenePos());
                _editedConnection.plugOutputId = plug->getPlugId();
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
            _editedNode.item = static_cast<AbstractNodeView *>(item);
            _editedNode.initClickPos = event->scenePos();
            _editedNode.initNodePose = _editedNode.item->pos();
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
                    if(_model->findInputPlug(plugUnderMouse->getPlugId()))
                    {
                        _editedConnection.item->setInput(plugUnderMouse->mapToScene(QPointF(0, 0)));
                        _editedConnection.plugInputId = plugUnderMouse->getPlugId();
                        cursor = Qt::PointingHandCursor;
                        plugFound = true;
                    }
                }
                else
                {
                    if(_model->findOutputPlug(plugUnderMouse->getPlugId()))
                    {
                        _editedConnection.item->setOutput(plugUnderMouse->mapToScene(QPointF(0, 0)));
                        _editedConnection.plugOutputId = plugUnderMouse->getPlugId();
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
                _editedConnection.plugInputId = QUuid();
            }
            else
            {
                _editedConnection.item->setOutput(event->scenePos());
                _editedConnection.plugOutputId = QUuid();
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
            Connection connection = _model->getConnection(connectionItem->getConnectionId());

            if(_editedNode.item->getNode()->hasInput(connection.input))
            {
                foreach(PlugItem *plugItem, _editedNode.item->getInputs())
                {
                    if(plugItem->getPlugId() == connection.input)
                    {
                        connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                        break;
                    }
                }
            }

            if(_editedNode.item->getNode()->hasOutput(connection.output))
            {
                foreach(PlugItem *plugItem, _editedNode.item->getOutputs())
                {
                    if(plugItem->getPlugId() == connection.output)
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
            if(item->type() == CustomItems::Plug)
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
        if(not _editedConnection.plugInputId.isNull() &&
           not _editedConnection.plugOutputId.isNull())
        {
            _model->addConnection(_editedConnection.plugOutputId, _editedConnection.plugInputId);
        }

        removeItem(_editedConnection.item);
        delete _editedConnection.item;

        _editedConnection.item = NULL;
        _editedConnection.plugInputId = QUuid();
        _editedConnection.plugOutputId = QUuid();
    }
    else if(_editedNode.item)
    {
        _editedNode.item = NULL;
        event->widget()->setCursor(Qt::OpenHandCursor);
    }
}

void ComposerScene::onConnectionAdded(const QUuid &connectionId)
{
    Connection connection = _model->getConnection(connectionId);
    ConnectionItem *connectionItem = new ConnectionItem();
    connectionItem->setConnectionId(connectionId);

    foreach(const AbstractNodeView *nodeView, _nodes)
    {
        foreach(const PlugItem *plugItem, nodeView->getInputs())
        {
            if(plugItem->getPlugId() == connection.input)
            {
                connectionItem->setInput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
        foreach(const PlugItem *plugItem, nodeView->getOutputs())
        {
            if(plugItem->getPlugId() == connection.output)
            {
                connectionItem->setOutput(plugItem->mapToScene(QPointF(0, 0)));
                break;
            }
        }
    }

    addItem(connectionItem);
    _connections << connectionItem;
}

void ComposerScene::onConnectionRemoved(const QUuid &connectionId)
{
    foreach(ConnectionItem *connectionItem, _connections)
    {
        if(connectionItem->getConnectionId() == connectionId)
        {
            removeItem(connectionItem);
            delete connectionItem;
            _connections.removeAll(connectionItem);
            break;
        }
    }
}

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

#include "nodestypesmanager.h"
#include "nodes/abstractnode.h"
#include "nodes/imagepreviewernode.h"
#include "nodesviews/abstractnodeview.h"
#include "nodesviews/connectionitem.h"
#include "nodesviews/imagepreview.h"
#include "nodesviews/plugitem.h"


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent),
    _editedConnection(NULL)
{
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
            node->setParent(this);
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
        }
    }
}

void ComposerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    if(item->type() == QGraphicsItem::UserType + 1)
    {
        PlugItem *plug = static_cast<PlugItem *>(item);

        _editedConnection = new ConnectionItem();
        addItem(_editedConnection);

        AbstractNodeView *node = static_cast<AbstractNodeView *>(item->parentItem());
        if(node->getInputs().contains(plug))
        {
            _editedConnection->setStartPlug(plug);
            _editedConnection->setEnd(event->scenePos());
            _editedConnectionFromStart = true;
        }
        else if(node->getOutputs().contains(plug))
        {
            _editedConnection->setStart(event->scenePos());
            _editedConnection->setEndPlug(plug);
            _editedConnectionFromStart = false;
        }
        else
        {
            qCritical("Selected plug not found in node inputs/outputs");
        }
    }
}

void ComposerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_editedConnection)
    {
        QGraphicsItem *itemUnderMouse = itemAt(event->scenePos(), QTransform());
        if(itemUnderMouse &&
           itemUnderMouse != _editedConnection &&
           itemUnderMouse->type() == QGraphicsItem::UserType + 1)
        {
            PlugItem *plug = static_cast<PlugItem *>(itemUnderMouse);

            AbstractNodeView *node = static_cast<AbstractNodeView *>(itemUnderMouse->parentItem());
            if(node->getOutputs().contains(plug) && _editedConnectionFromStart)
            {
                _editedConnection->setEndPlug(plug);
                return;
            }
            if(node->getInputs().contains(plug) && not _editedConnectionFromStart)
            {
                _editedConnection->setStartPlug(plug);
                return;
            }
        }

        if(_editedConnectionFromStart)
        {
            _editedConnection->setEnd(event->scenePos());
        }
        else
        {
            _editedConnection->setStart(event->scenePos());
        }
    }
}

void ComposerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_editedConnection)
    {
        if(_editedConnection->getStartPlug() && _editedConnection->getEndPlug())
        {
            // Update model
        }
        else
        {
            removeItem(_editedConnection);
            delete _editedConnection;
        }

        _editedConnection = NULL;
    }
}

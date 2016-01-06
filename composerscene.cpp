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

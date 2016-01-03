#include "composerscene.h"

#include <QDebug>
#include <QMimeData>
#include <QGraphicsRectItem>

#include "nodestypesmanager.h"
#include "nodes/abstractnode.h"
#include "nodes/imagepreviewernode.h"
#include "nodesviews/abstractnodeview.h"
#include "nodesviews/imagepreview.h"


ComposerScene::ComposerScene(QObject *parent) :
    QGraphicsScene(parent)
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

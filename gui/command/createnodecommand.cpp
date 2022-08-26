#include "createnodecommand.h"

#include "model/composermodel.h"
#include "gui/composerscene.h"
#include "gui/genericnodeitem.h"


CreateNodeCommand::CreateNodeCommand(ComposerScene *scene, const QString &nodeName, const QPointF &pos) :
    QUndoCommand("Create processor"),
    _scene(scene),
    _nodeName(nodeName),
    _nodeUid(QUuid::createUuid()),
    _pos(pos)
{
}

void CreateNodeCommand::redo()
{
    _scene->addNode(_nodeName, _nodeUid)->setPos(_pos);
}

void CreateNodeCommand::undo()
{
    auto node = _scene->accessModel()->findNode(_nodeUid);
    if(node)
    {
        _scene->accessModel()->removeNode(node);
    }
}

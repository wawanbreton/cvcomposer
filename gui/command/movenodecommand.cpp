#include "movenodecommand.h"

#include "gui/composerscene.h"
#include "gui/genericnodeitem.h"


MoveNodeCommand::MoveNodeCommand(ComposerScene *composerScene,
                                 const QUuid &nodeUid,
                                 const QPointF &oldPos,
                                 const QPointF &newPos) :
    QUndoCommand("Move processor"),
    _composerScene(composerScene),
    _nodeUid(nodeUid),
    _oldPos(oldPos),
    _newPos(newPos)
{
}

bool MoveNodeCommand::mergeWith(const QUndoCommand *other)
{
    if(other->id() == id())
    {
        auto moveNodeCommand = static_cast<const MoveNodeCommand *>(other);

        if(moveNodeCommand->_nodeUid == _nodeUid)
        {
            _newPos = moveNodeCommand->_newPos;
            setObsolete(_newPos == _oldPos);
            return true;
        }
    }

    return false;
}

void MoveNodeCommand::moveToPos(const QPointF &pos)
{
    _composerScene->moveNode(_nodeUid, pos);
}

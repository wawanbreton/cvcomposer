#pragma once

#include <QUndoCommand>

#include <QPointF>
#include <QUuid>

#include "gui/command/commandid.h"

class ComposerScene;

class MoveNodeCommand : public QUndoCommand
{
    public:
        explicit MoveNodeCommand(ComposerScene *composerScene,
                                 const QUuid &nodeUid,
                                 const QPointF &oldPos,
                                 const QPointF &newPos);

        virtual void redo() override
        { moveToPos(_newPos); }

        virtual void undo() override
        { moveToPos(_oldPos); }

        virtual int id() const override
        { return static_cast<int>(CommandId::MoveNode); }

        virtual bool mergeWith(const QUndoCommand *other) override;

    private:
        void moveToPos(const QPointF &pos);

    private:
        ComposerScene *const _composerScene;
        const QUuid _nodeUid;
        const QPointF _oldPos;
        QPointF _newPos;
};

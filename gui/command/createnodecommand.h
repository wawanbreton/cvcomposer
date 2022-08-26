#pragma once

#include <QUndoCommand>

#include <QDomElement>
#include <QUuid>
#include <QPointF>

class ComposerScene;
class GenericNodeItem;

class CreateNodeCommand : public QUndoCommand
{
    public:
        explicit CreateNodeCommand(ComposerScene *scene, const QString &nodeName, const QPointF &pos);

        virtual void redo() override;

        virtual void undo() override;

    private:
        ComposerScene *const _scene;
        const QString _nodeName;
        const QUuid _nodeUid;
        const QPointF _pos;
};

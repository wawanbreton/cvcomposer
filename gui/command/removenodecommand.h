#pragma once

#include <QUndoCommand>

#include <QDomDocument>
#include <QUuid>

class ComposerScene;
class GenericNodeItem;

class RemoveNodeCommand : public QUndoCommand
{
    public:
        virtual void redo() override;

        virtual void undo() override;

        static QUndoCommand *makeMetaCommand(ComposerScene *scene, GenericNodeItem *item);

    private:
        explicit RemoveNodeCommand(ComposerScene *scene, GenericNodeItem *item, QUndoCommand *parent);

    private:
        ComposerScene *const _scene;
        QUuid _nodeUid;
        QDomDocument _nodeDescDoc;
};


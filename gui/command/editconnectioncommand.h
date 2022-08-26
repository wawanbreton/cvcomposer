#pragma once

#include <QUndoCommand>

#include <QUuid>

class Connection;
class ComposerModel;
class Node;
class Plug;

class EditConnectionCommand : public QUndoCommand
{
    public:
        virtual void redo() override;

        virtual void undo() override;

        static QUndoCommand *makeMetaCommand(ComposerModel *model,
                                             const Connection *previousConnection,
                                             Plug *newPlugOutput,
                                             Plug *newPlugInput,
                                             QUndoCommand *parent = nullptr);

    private:
        enum class Action
        {
            Create,
            Delete,
        };

        enum class PlugSource
        {
            Input,
            Output
        };

    private:
        explicit EditConnectionCommand(ComposerModel *model,
                                       const QUuid &sourceNodeUid,
                                       const QString &sourceNodeOutput,
                                       const QUuid &targetNodeUid,
                                       const QString &targetNodeInput,
                                       Action action,
                                       QUndoCommand *parent = nullptr);
        void createConnection();

        void removeConnection();

        std::tuple<Plug *, Plug *> findPlugs() const;

        Plug *findPlug(const QUuid &nodeUid, const QString &plugName, PlugSource source) const;

        static QUndoCommand *makeCommand(ComposerModel *model,
                                         Node *nodeOutput,
                                         Plug *plugOutput,
                                         Node *nodeInput,
                                         Plug *plugInput,
                                         Action action,
                                         QUndoCommand *parent = nullptr);

        static QUndoCommand *makeCommand(ComposerModel *model,
                                         Plug *plugOutput,
                                         Plug *plugInput,
                                         Action action,
                                         QUndoCommand *parent = nullptr);

    private:
        ComposerModel *const _model;
        const QUuid _sourceNodeUid;
        const QString _sourceNodeOutput;
        const QUuid _targetNodeUid;
        const QString _targetNodeInput;
        const Action _action;
};


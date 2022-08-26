#pragma once

#include <QUndoCommand>

#include <QDateTime>
#include <QUuid>
#include <QVariant>

#include "gui/command/commandid.h"

class ComposerModel;

class EditValueCommand : public QUndoCommand
{
    public:
        EditValueCommand(ComposerModel *model,
                         const QUuid &nodeUid,
                         const QString &propertyName,
                         const QVariant &oldValue,
                         const QVariant &newValue);

        virtual void redo() override
        { setValue(_newValue); }

        virtual void undo() override
        { setValue(_oldValue); }

        virtual int id() const override
        { return static_cast<int>(CommandId::MoveNode); }

        virtual bool mergeWith(const QUndoCommand *other) override;

    private:
        void setValue(const QVariant &value);

    private:
        static constexpr std::chrono::milliseconds maxMergeDuration{2000};

    private:
        ComposerModel *const _model;
        const QUuid _nodeUid;
        const QString _propertyName;
        const QVariant _oldValue;
        QVariant _newValue;
        QDateTime _lastEditTimestamp;
};


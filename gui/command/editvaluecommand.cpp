#include "editvaluecommand.h"

#include "model/composermodel.h"
#include "model/node.h"


EditValueCommand::EditValueCommand(ComposerModel *model,
                                   const QUuid &nodeUid,
                                   const QString &propertyName,
                                   const QVariant &oldValue,
                                   const QVariant &newValue) :
    QUndoCommand("Set parameter"),
    _model(model),
    _nodeUid(nodeUid),
    _propertyName(propertyName),
    _oldValue(oldValue),
    _newValue(newValue),
    _lastEditTimestamp(QDateTime::currentDateTimeUtc())
{
}

bool EditValueCommand::mergeWith(const QUndoCommand *other)
{
    if(other->id() == id())
    {
        auto editValueCommand = static_cast<const EditValueCommand *>(other);

        if(editValueCommand->_nodeUid == _nodeUid &&
           editValueCommand->_propertyName == _propertyName)
        {
            QDateTime now = QDateTime::currentDateTimeUtc();
            if(_lastEditTimestamp.msecsTo(now) <= maxMergeDuration.count())
            {
                _newValue = editValueCommand->_newValue;
                _lastEditTimestamp = now;
                setObsolete(_newValue == _oldValue);
                return true;
            }
        }
    }

    return false;
}

void EditValueCommand::setValue(const QVariant &value)
{
    auto node = _model->findNode(_nodeUid);
    if(node)
    {
        node->setProperty(_propertyName, value);
    }
}

#include "editconnectioncommand.h"

#include "model/composermodel.h"
#include "model/connection.h"
#include "model/node.h"


EditConnectionCommand::EditConnectionCommand(ComposerModel *model,
                                             const QUuid &sourceNodeUid,
                                             const QString &sourceNodeOutput,
                                             const QUuid &targetNodeUid,
                                             const QString &targetNodeInput,
                                             Action action,
                                             QUndoCommand *parent) :
    QUndoCommand(action == Action::Create ? "Connect processors" : "Disconnect processors", parent),
    _model(model),
    _sourceNodeUid(sourceNodeUid),
    _sourceNodeOutput(sourceNodeOutput),
    _targetNodeUid(targetNodeUid),
    _targetNodeInput(targetNodeInput),
    _action(action)
{
}

void EditConnectionCommand::redo()
{
    switch(_action)
    {
        case Action::Create:
            createConnection();
            break;

        case Action::Delete:
            removeConnection();
            break;
    }
}

void EditConnectionCommand::undo()
{
    switch(_action)
    {
        case Action::Create:
            removeConnection();
            break;

        case Action::Delete:
            createConnection();
            break;
        }
}

QUndoCommand *EditConnectionCommand::makeMetaCommand(ComposerModel *model,
                                                     const Connection *previousConnection,
                                                     Plug *newPlugOutput,
                                                     Plug *newPlugInput)
{
    QUndoCommand *metaCommand = new QUndoCommand();

    bool doSomething = false;

    if(previousConnection)
    {
        // Edition : it has changed if any of the plug is different
        doSomething = previousConnection->getOutput() != newPlugOutput ||
                  previousConnection->getInput() != newPlugInput;
    }
    else
    {
        // Creation : it is valid if the two plugs are set and a similar connection does not exist
        if(newPlugInput && newPlugOutput)
        {
            doSomething = !model->findConnection(newPlugOutput, newPlugInput);
        }
    }

    if(doSomething)
    {
        bool deletePrevious = false;
        bool createNew = false;

        if(previousConnection)
        {
            deletePrevious = true;
            makeCommand(model,
                        previousConnection->getOutput(),
                        previousConnection->getInput(),
                        Action::Delete,
                        metaCommand);
        }

        if(newPlugInput && newPlugOutput)
        {
            auto sourceNode = model->findOutputPlug(newPlugOutput);
            auto targetNode = model->findInputPlug(newPlugInput);

            if(sourceNode && targetNode)
            {
                createNew = true;

                // Find the possible other connection that may be destroyed by this creation
                auto connection = model->findConnection(nullptr, newPlugInput);
                if(connection)
                {
                    makeCommand(model, connection->getOutput(), connection->getInput(), Action::Delete, metaCommand);
                }

                makeCommand(model, sourceNode, newPlugOutput, targetNode, newPlugInput, Action::Create, metaCommand);
            }
        }

        if(deletePrevious)
        {
            if(createNew)
            {
                metaCommand->setText("Edit connection");
            }
            else
            {
                metaCommand->setText("Delete connection");
            }
        }
        else
        {
            metaCommand->setText("Create connection");
        }
    }

    if(metaCommand->childCount() > 0)
    {
        return metaCommand;
    }
    else
    {
        delete metaCommand;
        return nullptr;
    }
}

void EditConnectionCommand::createConnection()
{
    Plug *plugSource;
    Plug *plugTarget;
    std::tie(plugSource, plugTarget) = findPlugs();

    if(plugSource && plugTarget)
    {
        _model->addConnection(plugSource, plugTarget);
    }
}

void EditConnectionCommand::removeConnection()
{
    Plug *plugSource;
    Plug *plugTarget;
    std::tie(plugSource, plugTarget) = findPlugs();

    if(plugSource && plugTarget)
    {
        auto connection  = _model->findConnection(plugSource, plugTarget);
        if(connection)
        {
            _model->removeConnection(connection);
        }
    }
}

std::tuple<Plug *, Plug *> EditConnectionCommand::findPlugs() const
{
    return {findPlug(_sourceNodeUid, _sourceNodeOutput, PlugSource::Output),
            findPlug(_targetNodeUid, _targetNodeInput, PlugSource::Input)};
}

Plug *EditConnectionCommand::findPlug(const QUuid &nodeUid, const QString &plugName, PlugSource source) const
{
    Plug *plug = nullptr;

    auto node = _model->findNode(nodeUid);
    if(node)
    {
        switch(source)
        {
            case PlugSource::Input:
                plug = node->findInput(plugName);
                break;

            case PlugSource::Output:
                plug = node->findOutput(plugName);
                break;
        }
    }

    return plug;
}

QUndoCommand *EditConnectionCommand::makeCommand(ComposerModel *model,
                                                 Node *nodeOutput,
                                                 Plug *plugOutput,
                                                 Node *nodeInput,
                                                 Plug *plugInput,
                                                 Action action,
                                                 QUndoCommand *parent)
{
     return new EditConnectionCommand(model,
                                      nodeOutput->getUid(),
                                      plugOutput->getDefinition().name,
                                      nodeInput->getUid(),
                                      plugInput->getDefinition().name,
                                      action,
                                      parent);
}

QUndoCommand *EditConnectionCommand::makeCommand(ComposerModel *model,
                                                 Plug *plugOutput,
                                                 Plug *plugInput,
                                                 Action action,
                                                 QUndoCommand *parent)
{
    auto sourceNode = model->findOutputPlug(plugOutput);
    auto targetNode = model->findInputPlug(plugInput);

    if(sourceNode && targetNode)
    {
        return makeCommand(model, sourceNode, plugOutput, targetNode, plugInput, action, parent);
    }

    return nullptr;
}


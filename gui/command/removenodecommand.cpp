#include "removenodecommand.h"

#include "model/node.h"
#include "model/composermodel.h"
#include "model/connection.h"
#include "gui/composerscene.h"
#include "gui/command/editconnectioncommand.h"
#include "gui/genericnodeitem.h"


RemoveNodeCommand::RemoveNodeCommand(ComposerScene *scene, GenericNodeItem *item, QUndoCommand *parent) :
    QUndoCommand("Delete processor", parent),
    _scene(scene),
    _nodeUid(item->getNode()->getUid())
{
    QDomElement nodeDescNode = _nodeDescDoc.createElement("node");
    scene->saveNode(_nodeDescDoc, nodeDescNode, item);
    _nodeDescDoc.appendChild(nodeDescNode);
}

void RemoveNodeCommand::redo()
{
    auto node = _scene->accessModel()->findNode(_nodeUid);
    if(node)
    {
        _scene->accessModel()->removeNode(node);
    }
}

void RemoveNodeCommand::undo()
{
    _scene->loadNode(_nodeDescDoc.firstChildElement());
}

QUndoCommand *RemoveNodeCommand::makeMetaCommand(ComposerScene *scene, GenericNodeItem *item)
{
    QUndoCommand *metaCommand = new QUndoCommand();
    auto node = item->accessNode();

    // First, remove all the connections to/from this node
    for(const Connection *connection : scene->getModel()->getConnections())
    {
        if(node->getInputs().contains(connection->getInput()) ||
           node->getOutputs().contains(connection->getOutput()))
        {
            EditConnectionCommand::makeMetaCommand(scene->accessModel(), connection, nullptr, nullptr, metaCommand);
        }
    }

    auto removeNodeCommand = new RemoveNodeCommand(scene, item, metaCommand);

    metaCommand->setText(removeNodeCommand->text());

    return metaCommand;
}

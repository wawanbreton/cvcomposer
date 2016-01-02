#include "abstractnode.h"


AbstractNode::AbstractNode(quint8 nbInputs, quint8 nbOutputs, const QString &userReadableName, QObject *parent) :
    QObject(parent),
    _nbInputs(nbInputs),
    _nbOutputs(nbOutputs),
    _userReadableName(userReadableName)
{
}

quint8 AbstractNode::getNbInputs() const
{
    return _nbInputs;
}

quint8 AbstractNode::getNbOutputs() const
{
    return _nbOutputs;
}

const QString &AbstractNode::getUserReadableName() const
{
    return _userReadableName;
}

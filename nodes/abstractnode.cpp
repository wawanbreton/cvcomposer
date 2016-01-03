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

QList<cv::Mat> AbstractNode::process(const QList<cv::Mat> &inputs)
{
    Q_ASSERT(inputs.count() == _nbInputs);
    QList<cv::Mat> outputs = processImpl(inputs);
    Q_ASSERT(outputs.count() == _nbOutputs);

    emit processDone(outputs, inputs);
    return outputs;
}

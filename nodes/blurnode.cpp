#include "blurnode.h"


BlurNode::BlurNode(QObject *parent) :
    AbstractNode(1, 1, tr("Blur"), parent)
{
}

QList<cv::Mat> BlurNode::process(const QList<cv::Mat> &inputs)
{
    cv::Mat blurred = inputs[0].clone();
    cv::blur(inputs[0], blurred, cv::Size(3, 3), cv::Point(-1,-1));
    return QList<cv::Mat>() << blurred;
}

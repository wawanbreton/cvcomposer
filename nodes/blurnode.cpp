#include "blurnode.h"

#include "opencv2/imgproc/imgproc.hpp"


BlurNode::BlurNode(QObject *parent) :
    AbstractNode(1, 1, parent)
{
}

QList<cv::Mat> BlurNode::process(const QList<cv::Mat> &inputs)
{
    cv::Mat blurred = inputs[0].clone();
    cv::blur(inputs[0], blurred, cv::Size(3, 3), cv::Point(-1,-1));
    return QList<cv::Mat>() << blurred;
}

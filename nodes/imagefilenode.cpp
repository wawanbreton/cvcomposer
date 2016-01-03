#include "imagefilenode.h"


ImageFileNode::ImageFileNode(QObject *parent) :
    AbstractNode(0, 1, tr("Image file"), parent)
{
}

QList<cv::Mat> ImageFileNode::process(const QList<cv::Mat> &inputs)
{
    Q_UNUSED(inputs);

    return QList<cv::Mat>() << cv::imread("4.jpg");
}

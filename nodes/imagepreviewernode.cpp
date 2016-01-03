#include "imagepreviewernode.h"


ImagePreviewerNode::ImagePreviewerNode(QObject *parent) :
    AbstractNode(1, 0, tr("Preview"), parent)
{
}

QList<cv::Mat> ImagePreviewerNode::process(const QList<cv::Mat> &inputs)
{
    cv::Mat currentImage = inputs[0];

    return QList<cv::Mat>();
}

#include "imagefilenode.h"

#include <opencv2/highgui/highgui.hpp>


ImageFileNode::ImageFileNode(QObject *parent) :
    AbstractNode(0, 1, tr("Image file"), parent)
{
}

QList<cv::Mat> ImageFileNode::processImpl(const QList<cv::Mat> &inputs)
{
    Q_UNUSED(inputs);

    return QList<cv::Mat>() << cv::imread("4.jpg", CV_LOAD_IMAGE_COLOR);
}

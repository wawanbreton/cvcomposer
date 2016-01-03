#ifndef IMAGEPREVIEWERNODE_H
#define IMAGEPREVIEWERNODE_H

#include "nodes/abstractnode.h"

class ImagePreviewerNode : public AbstractNode
{
    Q_OBJECT

    public:
        explicit ImagePreviewerNode(QObject *parent = NULL);

        virtual QList<cv::Mat> process(const QList<cv::Mat> &inputs);
};

#endif // IMAGEPREVIEWERNODE_H

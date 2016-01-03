#ifndef IMAGEFILENODE_H
#define IMAGEFILENODE_H

#include "nodes/abstractnode.h"

class ImageFileNode : public AbstractNode
{
    Q_OBJECT

    public:
        explicit ImageFileNode(QObject *parent = NULL);

        virtual QList<cv::Mat> processImpl(const QList<cv::Mat> &inputs);
};

#endif // IMAGEFILENODE_H

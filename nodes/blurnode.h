#ifndef BLURNODE_H
#define BLURNODE_H

#include "nodes/abstractnode.h"

class BlurNode : public AbstractNode
{
    Q_OBJECT

    public:
        explicit BlurNode(QObject *parent = NULL);

        virtual QList<cv::Mat> process(const QList<cv::Mat> &inputs);
};

#endif // BLURNODE_H

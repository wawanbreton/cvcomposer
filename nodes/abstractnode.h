#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QObject>

#include <opencv2/opencv.hpp>

class AbstractNode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractNode(quint8 nbInputs, quint8 nbOutputs, QObject *parent = NULL);

        virtual QList<cv::Mat> process(const QList<cv::Mat> &inputs) = 0;
};

#endif // ABSTRACTNODE_H

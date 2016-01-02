#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QObject>

#include <opencv2/opencv.hpp>

class AbstractNode : public QObject
{
    Q_OBJECT

    public:
        explicit AbstractNode(quint8 nbInputs, quint8 nbOutputs, const QString &userReadableName, QObject *parent = NULL);

        quint8 getNbInputs() const;

        quint8 getNbOutputs() const;

        const QString &getUserReadableName() const;

        virtual QList<cv::Mat> process(const QList<cv::Mat> &inputs) = 0;

    private:
        const quint8 _nbInputs;
        const quint8 _nbOutputs;
        QString _userReadableName;
};

#endif // ABSTRACTNODE_H

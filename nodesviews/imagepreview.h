#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include "nodesviews/abstractnodeview.h"
#include <QObject>

#include <opencv2/core/core.hpp>

class ImagePreview : public AbstractNodeView
{
    Q_OBJECT

    public:
        ImagePreview(AbstractNode *node, QGraphicsItem *parent = NULL);

        virtual QRectF boundingRect() const;

    protected:
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private slots:
        void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

    private:
        cv::Mat _mat;
};

#endif // IMAGEPREVIEW_H

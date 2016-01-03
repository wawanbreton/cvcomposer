#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include "nodesviews/abstractnodeview.h"
#include <QObject>

class ImagePreview : public QObject, public AbstractNodeView
{
    Q_OBJECT

    public:
        ImagePreview(AbstractNode *node, QGraphicsItem *parent = NULL);

        virtual QRectF boundingRect() const;

    private slots:

};

#endif // IMAGEPREVIEW_H

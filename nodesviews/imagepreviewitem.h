// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#ifndef IMAGEPREVIEWITEM_H
#define IMAGEPREVIEWITEM_H

#include "nodesviews/genericnodeitem.h"

#include <opencv2/core/core.hpp>

class ImagePreviewItem : public GenericNodeItem
{
    Q_OBJECT

    public:
        ImagePreviewItem(AbstractNode *node, QGraphicsItem *parent = NULL);

        virtual QRectF boundingRect() const;

    protected:
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private slots:
        void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void onProcessUnavailable();

    private:
        QPixmap _image;
};

#endif // IMAGEPREVIEWITEM_H

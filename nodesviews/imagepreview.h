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

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

#ifndef IMAGEVIEWERITEM_H
#define IMAGEVIEWERITEM_H

#include "nodesviews/genericnodeitem.h"

#include <opencv2/core/core.hpp>

class ImageViewerDockWidget;

class ImageViewerItem : public GenericNodeItem
{
    Q_OBJECT

    public:
        ImageViewerItem(AbstractNode *node, QGraphicsItem *parent = NULL);

    private slots:
        void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        void onProcessUnavailable();

    private:
        ImageViewerDockWidget *_dockWidget;
};

#endif // IMAGEVIEWERITEM_H

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

#ifndef IMAGEDOCKVIEWER_H
#define IMAGEDOCKVIEWER_H

#include "nodesviews/abstractnodeview.h"

#include <opencv2/core/core.hpp>

class ImageDockWidget;

class ImageDockViewer : public AbstractNodeView
{
    Q_OBJECT

    public:
        ImageDockViewer(AbstractNode *node, QGraphicsItem *parent = NULL);

    private slots:
        void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

    private:
        ImageDockWidget *_dockWidget;
};

#endif // IMAGEDOCKVIEWER_H

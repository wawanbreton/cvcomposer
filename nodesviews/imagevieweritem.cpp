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

#include "imagevieweritem.h"

#include <QApplication>

#include "nodes/abstractnode.h"
#include "nodesviews/imageviewerdockwidget.h"
#include "nodesviews/imageviewerwidget.h"


ImageViewerItem::ImageViewerItem(AbstractNode *node, QGraphicsItem *parent) :
    GenericNodeItem(node, parent),
    _dockWidget(new ImageViewerDockWidget(QApplication::activeWindow()))
{
    QString title = "Image viewer";

    _dockWidget->show();
    _dockWidget->setWindowTitle(title);

    ImageViewerWidget *widget = new ImageViewerWidget(title);
    setWidget(widget);
    connect(widget, SIGNAL(nameChanged(QString)), _dockWidget, SLOT(setWindowTitle(QString)));

    connect(node, SIGNAL(processDone(QList<cv::Mat>, QList<cv::Mat>)),
                  SLOT(onProcessDone(QList<cv::Mat>, QList<cv::Mat>)));

    connect(node, SIGNAL(processUnavailable()), SLOT(onProcessUnavailable()));
}

void ImageViewerItem::onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Viewer has no ouput, it only displays the input image
    _dockWidget->setImage(inputs[0]);
}

void ImageViewerItem::onProcessUnavailable()
{
    _dockWidget->setImage(cv::Mat());
}

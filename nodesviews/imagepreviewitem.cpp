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

#include "imagepreviewitem.h"

#include <QDebug>
#include <QPainter>

#include "nodes/abstractnode.h"
#include "nodesviews/plugitem.h"
#include "cvutils.h"


ImagePreviewItem::ImagePreviewItem(AbstractNode *node, QGraphicsItem *parent) :
    GenericNodeItem(node, parent),
    _image()
{
    connect(node, SIGNAL(processDone(QList<cv::Mat>, QList<cv::Mat>)),
                  SLOT(onProcessDone(QList<cv::Mat>, QList<cv::Mat>)));

    connect(node, SIGNAL(processUnavailable()), SLOT(onProcessUnavailable()));
}

QRectF ImagePreviewItem::boundingRect() const
{
    QRectF parent = GenericNodeItem::boundingRect();
    parent.setHeight(parent.width());
    return parent;
}

void ImagePreviewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GenericNodeItem::paint(painter, option, widget);

    QRectF rect = boundingRect().adjusted(2 * PlugItem::radius, 25, -2 * PlugItem::radius, -5);

    if(_image.isNull())
    {
        painter->fillRect(rect, Qt::gray);
    }
    else
    {
        painter->drawPixmap(rect, _image, _image.rect());
    }
}

void ImagePreviewItem::onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Previewer has no ouput, it only displays the input image

    _image = QPixmap::fromImage(CvUtils::toQImage(inputs[0]));

    update();
}

void ImagePreviewItem::onProcessUnavailable()
{
    _image = QPixmap();
    update();
}

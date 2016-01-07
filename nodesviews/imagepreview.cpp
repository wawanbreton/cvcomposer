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

#include "imagepreview.h"

#include <QDebug>
#include <QPainter>

#include "nodes/abstractnode.h"
#include "nodesviews/plugitem.h"


ImagePreview::ImagePreview(AbstractNode *node, QGraphicsItem *parent) :
    AbstractNodeView(node, parent),
    _mat()
{
    connect(node, SIGNAL(processDone(QList<cv::Mat>, QList<cv::Mat>)),
                  SLOT(onProcessDone(QList<cv::Mat>, QList<cv::Mat>)));
}

QRectF ImagePreview::boundingRect() const
{
    QRectF parent = AbstractNodeView::boundingRect();
    parent.setHeight(parent.width());
    return parent;
}

void ImagePreview::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    AbstractNodeView::paint(painter, option, widget);

    QRectF rect = boundingRect().adjusted(2 * PlugItem::radius, 25, -2 * PlugItem::radius, -5);

    if(_mat.total() == 0)
    {
        painter->fillRect(rect, Qt::gray);
    }
    else
    {
        QImage image;

        if(_mat.type() == CV_8UC1) // 8-bits unsigned, NO. OF CHANNELS=1
        {
            // Set the color table (used to translate colour indexes to qRgb values)
            QVector<QRgb> colorTable;
            for(int i = 0 ; i < 256; i++)
            {
                colorTable.push_back(qRgb(i,i,i));
            }

            // Create QImage with same dimensions as input Mat
            image = QImage((const uchar*)_mat.data, _mat.cols, _mat.rows, _mat.step, QImage::Format_Indexed8);
            image.setColorTable(colorTable);
        }
        if(_mat.type() == CV_8UC3) // 8-bits unsigned, NO. OF CHANNELS=3
        {
            image = QImage((const uchar*)_mat.data, _mat.cols, _mat.rows, _mat.step, QImage::Format_RGB888);
            image = image.rgbSwapped();
        }
        else
        {
            qWarning() << "Mat could not be converted to QImage.";
        }

        if(not image.isNull())
        {
            painter->drawImage(rect, image);
        }
    }
}

void ImagePreview::onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Previewer has no ouput, it only displays the input image
    _mat = inputs[0];
    update();
}

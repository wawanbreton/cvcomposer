#include "imagepreview.h"

#include <QDebug>
#include <QPainter>

#include "nodes/abstractnode.h"

#include <opencv2/highgui/highgui.hpp>


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

    QRectF rect = boundingRect().adjusted(2 * plugRadius, 25, -2 * plugRadius, -5);

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

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

#include "dataviewerwidget.h"

#include <QPainter>
#include <QVariant>
#include <QStyle>

#include "global/cvutils.h"


DataViewerWidget::DataViewerWidget(QWidget *parent) :
    AbstractPlugWidget(parent)
{
    setMinimumSize(128, 128);
}

void DataViewerWidget::onNodeProcessed(const Properties &inputs, const Properties &outputs)
{
    AbstractPlugWidget::onNodeProcessed(inputs, outputs);

    _image = QPixmap();
    _text.clear();

    const QVariant &output = outputs["output"];
    if(output.userType() == qMetaTypeId<QPixmap>())
    {
        _image = output.value<QPixmap>();
    }
    else if(output.userType() == qMetaTypeId<cv::Rect>())
    {
        cv::Rect rect = output.value<cv::Rect>();
        _text = QString("x: %1\ny: %2\nw: %3\nh:%4");
        _text = _text.arg(rect.x);
        _text = _text.arg(rect.y);
        _text = _text.arg(rect.width);
        _text = _text.arg(rect.height);
    }

    update();
}

void DataViewerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if(!_image.isNull())
    {
        qreal imageRatio = qreal(_image.width()) / _image.height();
        qreal widgetRatio = qreal(width()) / height();
        QRect rect;
        if(imageRatio > widgetRatio)
        {
            rect = QRect(0, 0, width(), width() / imageRatio);
            rect.translate(0, (height() - rect.height()) / 2);
        }
        else
        {
            rect = QRect(0, 0, height() * imageRatio, height());
            rect.translate((width() - rect.width()) / 2, 0);
        }
        painter.drawPixmap(rect, _image);
    }
    else if(!_text.isEmpty())
    {
        style()->drawItemText(&painter, rect(), Qt::AlignLeft | Qt::AlignVCenter, palette(), true, _text);
    }
    else
    {
        painter.fillRect(rect(), Qt::gray);
    }
}

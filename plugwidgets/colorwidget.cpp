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

#include "colorwidget.h"

#include <QColorDialog>
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "cvutils.h"


ColorWidget::ColorWidget(QWidget *parent) :
    AbstractPlugWidget(parent),
    _lineEdit(new QLineEdit(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_lineEdit);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
    layout->setSpacing(0);

    _lineEdit->setReadOnly(true);
    _lineEdit->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    _lineEdit->setFixedWidth(60);
}

QVariant ColorWidget::getValue() const
{
    QColor color = getColor();
    return QVariant::fromValue(cv::Scalar(color.blue(), color.green(), color.red(), 255));
}

void ColorWidget::setValue(const QVariant &value)
{
    cv::Scalar color = value.value<cv::Scalar>();
    setColor(QColor::fromRgb(qBound(0.0, color[2], 255.0),
                             qBound(0.0, color[1], 255.0),
                             qBound(0.0, color[0], 255.0),
                             255));
}

void ColorWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QColor color = QColorDialog::getColor(_lineEdit->palette().color(QPalette::Base));
        if(color.isValid())
        {
            setColor(color);
            emit valueChanged();
        }
    }
}

QColor ColorWidget::getColor() const
{
    return _lineEdit->palette().color(QPalette::Base);
}

void ColorWidget::setColor(const QColor &color)
{
    QPalette palette = _lineEdit->palette();
    palette.setColor(QPalette::Base, color);
    _lineEdit->setPalette(palette);
}

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

#include "sizewidget.h"

#include <QLabel>
#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "cvutils.h"


SizeWidget::SizeWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _spinBoxWidth(new QSpinBox(this)),
    _spinBoxHeight(new QSpinBox(this))
{
    QLabel *label = new QLabel(this);
    label->setText("x");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_spinBoxWidth);
    layout->addWidget(label);
    layout->addWidget(_spinBoxHeight);

    connect(_spinBoxWidth,  SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
    connect(_spinBoxHeight, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));

    for(auto iterator = properties.begin() ; iterator != properties.end() ; iterator++)
    {
        if(iterator.key().startsWith("width-"))
        {
            _spinBoxWidth->setProperty(iterator.key().mid(6).toUtf8(), iterator.value());
        }
        else if(iterator.key().startsWith("height-"))
        {
            _spinBoxHeight->setProperty(iterator.key().mid(7).toUtf8(), iterator.value());
        }
    }
}

QVariant SizeWidget::getValue() const
{
    cv::Size size;
    size.width = _spinBoxWidth->value();
    size.height = _spinBoxHeight->value();

    return QVariant::fromValue(size);
}

void SizeWidget::setValue(const QVariant &value)
{
    cv::Size size = value.value<cv::Size>();

    _spinBoxWidth->blockSignals(true);
    _spinBoxWidth->setValue(size.width);
    _spinBoxWidth->blockSignals(false);

    _spinBoxHeight->blockSignals(true);
    _spinBoxHeight->setValue(size.height);
    _spinBoxHeight->blockSignals(false);
}

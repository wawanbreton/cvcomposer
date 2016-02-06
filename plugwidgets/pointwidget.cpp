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

#include "pointwidget.h"

#include <QLabel>
#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "cvutils.h"


PointWidget::PointWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _spinBoxX(new QSpinBox(this)),
    _spinBoxY(new QSpinBox(this))
{
    QLabel *label = new QLabel(this);
    label->setText("x");

    _spinBoxX->setMaximum(9999);
    _spinBoxY->setMaximum(9999);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_spinBoxX);
    layout->addWidget(label);
    layout->addWidget(_spinBoxY);

    connect(_spinBoxX, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));
    connect(_spinBoxY, SIGNAL(valueChanged(int)), SIGNAL(valueChanged()));

    for(auto iterator = properties.begin() ; iterator != properties.end() ; iterator++)
    {
        if(iterator.key().startsWith("x-"))
        {
            _spinBoxX->setProperty(iterator.key().mid(2).toUtf8(), iterator.value());
        }
        else if(iterator.key().startsWith("y-"))
        {
            _spinBoxY->setProperty(iterator.key().mid(2).toUtf8(), iterator.value());
        }
    }
}

QVariant PointWidget::getValue() const
{
    cv::Point point;
    point.x = _spinBoxX->value();
    point.y = _spinBoxY->value();

    return QVariant::fromValue(point);
}

void PointWidget::setValue(const QVariant &value)
{
    cv::Point point = value.value<cv::Point>();

    _spinBoxX->blockSignals(true);
    _spinBoxX->setValue(point.x);
    _spinBoxX->blockSignals(false);

    _spinBoxY->blockSignals(true);
    _spinBoxY->setValue(point.y);
    _spinBoxY->blockSignals(false);
}

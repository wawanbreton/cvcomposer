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

#include "doublewidget.h"

#include <QHBoxLayout>

#include "global/cvconstants.h"


DoubleWidget::DoubleWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _spinBox(new QDoubleSpinBox(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_spinBox);

    _spinBox->setMaximum(CvConstants::defaultDoubleMax);

    connect(_spinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &DoubleWidget::onGuiValueChanged);

    properties.applyTo(_spinBox);
}

QVariant DoubleWidget::getValue() const
{
    return _spinBox->value();
}

void DoubleWidget::setValueImpl(const QVariant &value)
{
    _spinBox->setValue(value.toDouble());
}

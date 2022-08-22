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

#include "booleanwidget.h"

#include <QHBoxLayout>


BooleanWidget::BooleanWidget(QWidget *parent) :
    AbstractPlugWidget(parent),
    _checkBox(new QCheckBox(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_checkBox);

    connect(_checkBox, &QCheckBox::toggled, this, &BooleanWidget::valueChanged);
}

QVariant BooleanWidget::getValue() const
{
    return _checkBox->isChecked();
}

void BooleanWidget::setValue(const QVariant &value)
{
    _checkBox->setChecked(value.toBool());
}

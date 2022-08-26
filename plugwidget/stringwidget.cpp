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

#include "stringwidget.h"

#include <QHBoxLayout>


StringWidget::StringWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _lineEdit(new QLineEdit(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_lineEdit);

    connect(_lineEdit, &QLineEdit::textChanged, this, &StringWidget::onGuiValueChanged);

    for(auto iterator = properties.begin() ; iterator != properties.end() ; iterator++)
    {
        _lineEdit->setProperty(iterator.key().toUtf8(), iterator.value());
    }
}

QVariant StringWidget::getValue() const
{
    return _lineEdit->text();
}

void StringWidget::setValueImpl(const QVariant &value)
{
    _lineEdit->setText(value.toString());
}

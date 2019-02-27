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

#include "enumerationwidget.h"

#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "global/cvutils.h"


EnumerationWidget::EnumerationWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _comboBox(new QComboBox(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_comboBox);

    connect(_comboBox,  SIGNAL(currentIndexChanged(int)), SIGNAL(valueChanged()));

    for(auto iterator = properties.begin() ; iterator != properties.end() ; iterator++)
    {
        if(iterator.key() == "values")
        {
            auto values = iterator.value().value<QList<QPair<QString, QVariant> > >();
            for(int i = 0 ; i < values.count() ; i++)
            {
                if(values[i].first.isEmpty() || values[i].second.isNull())
                {
                    _comboBox->insertSeparator(i);
                }
                else
                {
                    _comboBox->addItem(values[i].first, values[i].second);
                }
            }
        }
        else
        {
            _comboBox->setProperty(iterator.key().toUtf8(), iterator.value());
        }
    }
}

QVariant EnumerationWidget::getValue() const
{
    return _comboBox->currentData();
}

void EnumerationWidget::setValue(const QVariant &value)
{
    _comboBox->setCurrentIndex(_comboBox->findData(value));
}

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

#include "abstractplugwidget.h"

#include <QDebug>
#include <QVariant>


AbstractPlugWidget::AbstractPlugWidget(QWidget *parent) :
    QWidget(parent)
{
}

void AbstractPlugWidget::setValueImpl(const QVariant &value)
{
    Q_UNUSED(value)
    qCritical() << "AbstractPlugWidget::setValueImpl" << "This method should not be called";
}

QVariant AbstractPlugWidget::getValue() const
{
    qCritical() << "AbstractPlugWidget::getValue" << "This method should not be called";
    return QVariant();
}

void AbstractPlugWidget::setValue(const QVariant &value)
{
    _settingValue = true;
    setValueImpl(value);
    _settingValue = false;
}

QMap<QString, QString> AbstractPlugWidget::save() const
{
    // Default is nothing to do
    return QMap<QString, QString>();
}

void AbstractPlugWidget::load(const QMap<QString, QString> &properties)
{
    // Default is nothing to do
    Q_UNUSED(properties)
}

void AbstractPlugWidget::onNodeProcessed(const Properties &inputs, const Properties &outputs)
{
    Q_UNUSED(inputs)
    Q_UNUSED(outputs)
    // Default is nothing to do
}

void AbstractPlugWidget::onGuiValueChanged()
{
    if(!_settingValue)
    {
        emit valueChanged();
    }
}

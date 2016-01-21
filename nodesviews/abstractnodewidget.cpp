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

#include "abstractnodewidget.h"

AbstractNodeWidget::AbstractNodeWidget(QWidget *parent) : QWidget(parent)
{

}

Properties AbstractNodeWidget::getProperties() const
{
    Properties properties;
    foreach(const QString &propertyName, getPropertiesNames())
    {
        properties.insert(propertyName, getProperty((propertyName)));
    }

    return properties;
}

void AbstractNodeWidget::onPropertyChanged(const QString &name)
{
    emit propertyChanged(name, getProperty(name));
}

void AbstractNodeWidget::onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs);
    Q_UNUSED(inputs);
}

void AbstractNodeWidget::onProcessUnavailable()
{
}


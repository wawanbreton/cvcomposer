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

QVariant AbstractPlugWidget::getValue() const
{
    qCritical() << "AbstractPlugWidget::getValue" << "This method should not be called";
    return QVariant();
}

void AbstractPlugWidget::onConnectedInputProcessed(const QVariant &value)
{
    Q_UNUSED(value);
    // Default is nothing to do
}

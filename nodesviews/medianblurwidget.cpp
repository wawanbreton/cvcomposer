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

#include "medianblurwidget.h"
#include "ui_medianblurwidget.h"

MedianBlurWidget::MedianBlurWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::MedianBlurWidget)
{
    _ui->setupUi(this);

    connect(_ui->spinBox, SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
}

MedianBlurWidget::MedianBlurWidget(const MedianBlurWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("MedianBlurWidget::MedianBlurWidget");
}

MedianBlurWidget::~MedianBlurWidget()
{
    delete _ui;
}

QVariant MedianBlurWidget::getProperty(const QString &name) const
{
    if(name == "size")
    {
        return _ui->spinBox->value();
    }

    return QVariant();
}

QStringList MedianBlurWidget::getPropertiesNames() const
{
    return QStringList() << "size";
}

void MedianBlurWidget::onSizeChanged()
{
    onPropertyChanged("size");
}

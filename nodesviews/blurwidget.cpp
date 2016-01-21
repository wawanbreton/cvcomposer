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

#include "blurwidget.h"
#include "ui_blurwidget.h"

#include "cvutils.h"


BlurWidget::BlurWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::BlurWidget)
{
    _ui->setupUi(this);

    connect(_ui->spinBoxWidth,  SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxHeight, SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxAnchorX, SIGNAL(valueChanged(int)), SLOT(onAnchorChanged()));
    connect(_ui->spinBoxAnchorY, SIGNAL(valueChanged(int)), SLOT(onAnchorChanged()));
}

BlurWidget::BlurWidget(const BlurWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("BlurWidget::BlurWidget");
}

BlurWidget::~BlurWidget()
{
    delete _ui;
}

QVariant BlurWidget::getProperty(const QString &name) const
{
    if(name == "size")
    {
        return QVariant::fromValue(cv::Size(_ui->spinBoxWidth->value(),
                                            _ui->spinBoxHeight->value()));
    }
    else if(name == "anchor")
    {
        return QVariant::fromValue(cv::Point(_ui->spinBoxAnchorX->value(),
                                             _ui->spinBoxAnchorY->value()));
    }

    return QVariant();
}

void BlurWidget::setProperty(const QString &name, const QVariant &value)
{
    if(name == "size")
    {
        cv::Size size = value.value<cv::Size>();
        _ui->spinBoxWidth->setValue(size.width);
        _ui->spinBoxHeight->setValue(size.height);
    }
    else if(name == "anchor")
    {
        cv::Point anchor = value.value<cv::Point>();
        _ui->spinBoxAnchorX->setValue(anchor.x);
        _ui->spinBoxAnchorY->setValue(anchor.y);
    }
}

QStringList BlurWidget::getPropertiesNames() const
{
    return QStringList() << "size" << "anchor";
}

void BlurWidget::onSizeChanged()
{
    onPropertyChanged("size");
}

void BlurWidget::onAnchorChanged()
{
    onPropertyChanged("anchor");
}

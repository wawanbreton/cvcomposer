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


BlurWidget::BlurWidget(const cv::Size &size, const cv::Point &anchor, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::BlurWidget)
{
    _ui->setupUi(this);

    _ui->spinBoxWidth->setValue(size.width);
    _ui->spinBoxHeight->setValue(size.height);
    _ui->spinBoxAnchorX->setValue(anchor.x);
    _ui->spinBoxAnchorY->setValue(anchor.y);

    connect(_ui->spinBoxWidth,  SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxHeight, SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxAnchorX, SIGNAL(valueChanged(int)), SLOT(onAnchorChanged()));
    connect(_ui->spinBoxAnchorY, SIGNAL(valueChanged(int)), SLOT(onAnchorChanged()));
}

BlurWidget::~BlurWidget()
{
    delete _ui;
}

void BlurWidget::onSizeChanged()
{
    emit sizeChanged(cv::Size(_ui->spinBoxWidth->value(), _ui->spinBoxHeight->value()));
}

void BlurWidget::onAnchorChanged()
{
    emit sizeChanged(cv::Point(_ui->spinBoxAnchorX->value(), _ui->spinBoxAnchorY->value()));
}

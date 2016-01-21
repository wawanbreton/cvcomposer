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

#include "gaussianblurwidget.h"
#include "ui_gaussianblurwidget.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "cvutils.h"


GaussianBlurWidget::GaussianBlurWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::GaussianBlurWidget)
{
    _ui->setupUi(this);

    connect(_ui->spinBoxWidth, SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxHeight, SIGNAL(valueChanged(int)), SLOT(onSizeChanged()));
    connect(_ui->spinBoxSigmaX, SIGNAL(valueChanged(double)), SLOT(onSigmaXChanged()));
    connect(_ui->spinBoxSigmaY, SIGNAL(valueChanged(double)), SLOT(onSigmaYChanged()));
    connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(onBorderChanged()));
}

GaussianBlurWidget::GaussianBlurWidget(const GaussianBlurWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("GaussianBlurWidget::GaussianBlurWidget");
}

GaussianBlurWidget::~GaussianBlurWidget()
{
    delete _ui;
}

QStringList GaussianBlurWidget::getPropertiesNames() const
{
    return QStringList() << "size" << "sigmaX" << "sigmaY" << "border";
}

QVariant GaussianBlurWidget::getProperty(const QString &name) const
{
    if(name == "size")
    {
        return QVariant::fromValue(cv::Size(_ui->spinBoxWidth->value(),
                                            _ui->spinBoxHeight->value()));
    }
    else if(name == "sigmaX")
    {
        return _ui->spinBoxSigmaX->value();
    }
    else if(name == "sigmaY")
    {
        return _ui->spinBoxSigmaY->value();
    }
    else if(name == "border")
    {
        switch(_ui->comboBox->currentIndex())
        {
            case 0:
                return cv::BORDER_REFLECT101;
            case 1:
                return cv::BORDER_REFLECT;
            case 2:
                return cv::BORDER_REPLICATE;
            case 3:
                return cv::BORDER_CONSTANT;
        }
    }

    return QVariant();
}

void GaussianBlurWidget::onSizeChanged()
{
    onPropertyChanged("size");
}

void GaussianBlurWidget::onSigmaXChanged()
{
    onPropertyChanged("sigmaX");
}

void GaussianBlurWidget::onSigmaYChanged()
{
    onPropertyChanged("sigmaY");
}

void GaussianBlurWidget::onBorderChanged()
{
    onPropertyChanged("border");
}

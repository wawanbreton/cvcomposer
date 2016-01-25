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

#include "bilateralfilterwidget.h"
#include "ui_bilateralfilterwidget.h"

#include <opencv2/imgproc/imgproc.hpp>


BilateralFilterWidget::BilateralFilterWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::BilateralFilterWidget)
{
    _ui->setupUi(this);

    connect(_ui->spinBoxDiameter, SIGNAL(valueChanged(int)), SLOT(onDiameterChanged()));
    connect(_ui->spinBoxSigmaColor, SIGNAL(valueChanged(double)), SLOT(onSigmaColorChanged()));
    connect(_ui->spinBoxSigmaSpace, SIGNAL(valueChanged(double)), SLOT(onSigmaSpaceChanged()));
    connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(onBorderChanged()));
}

BilateralFilterWidget::BilateralFilterWidget(const BilateralFilterWidget &other):
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("BilateralFilterWidget::BilateralFilterWidget");
}

BilateralFilterWidget::~BilateralFilterWidget()
{
    delete _ui;
}

QVariant BilateralFilterWidget::getProperty(const QString &name) const
{
    if(name == "diameter")
    {
        return _ui->spinBoxDiameter->value();
    }
    else if(name == "sigmaColor")
    {
        return _ui->spinBoxSigmaColor->value();
    }
    else if(name == "sigmaSpace")
    {
        return _ui->spinBoxSigmaSpace->value();
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

QStringList BilateralFilterWidget::getPropertiesNames() const
{
    return QStringList()  << "diameter" << "sigmaColor" << "sigmaSpace" << "border";
}

void BilateralFilterWidget::onDiameterChanged()
{
    onPropertyChanged("diameter");
}

void BilateralFilterWidget::onSigmaColorChanged()
{
    onPropertyChanged("sigmaColor");
}

void BilateralFilterWidget::onSigmaSpaceChanged()
{
    onPropertyChanged("sigmaSpace");
}

void BilateralFilterWidget::onBorderChanged()
{
    onPropertyChanged("border");
}

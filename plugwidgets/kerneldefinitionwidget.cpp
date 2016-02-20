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

#include "kerneldefinitionwidget.h"

#include <iostream>

#include <QDoubleSpinBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>

#include "cvconstants.h"
#include "cvutils.h"
#include "plugwidgets/doubleitemdelegate.h"


KernelDefinitionWidget::KernelDefinitionWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _table(new QTableWidget(this))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_table);

    _table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QDoubleSpinBox tmpSpinBox;
    tmpSpinBox.setMaximum(properties.value("maximum", CvConstants::defaultDoubleMax).toDouble());
    tmpSpinBox.setDecimals(properties.value("decimals", CvConstants::defaultDoubleDecimals).toInt());
    properties.applyTo(&tmpSpinBox);
    _table->horizontalHeader()->setDefaultSectionSize(tmpSpinBox.sizeHint().width());

    _table->verticalHeader()->setVisible(false);
    _table->horizontalHeader()->setVisible(false);

    _table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    DoubleItemDelegate *delegate = new DoubleItemDelegate(properties, this);
    _table->setItemDelegate(delegate);
    connect(delegate, SIGNAL(valueChanged()), SIGNAL(valueChanged()));
}

QSize KernelDefinitionWidget::sizeHint() const
{
    return QSize(_table->horizontalHeader()->length() + 4,
                 _table->verticalHeader()->length() + 4);
}

QVariant KernelDefinitionWidget::getValue() const
{
    cv::Mat mat(_table->rowCount(), _table->columnCount(), CV_64FC1);

    double *rowPointer;
    for(int row = 0 ; row < mat.rows ; row++)
    {
        rowPointer = mat.ptr<double>(row);
        for(int col = 0 ; col < mat.cols ; col++)
        {
            rowPointer[col] = _table->item(row, col)->data(Qt::EditRole).toDouble();
        }
    }

    return QVariant::fromValue(mat);
}

void KernelDefinitionWidget::setValue(const QVariant &value)
{
    cv::Mat mat = value.value<cv::Mat>();
    _table->setRowCount(mat.rows);
    _table->setColumnCount(mat.cols);

    double *rowPointer;
    for(int row = 0 ; row < mat.rows ; row++)
    {
        rowPointer = mat.ptr<double>(row);
        for(int col = 0 ; col < mat.cols ; col++)
        {
            double value = rowPointer[col];
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, value);
            _table->setItem(row, col, item);
        }
    }
}

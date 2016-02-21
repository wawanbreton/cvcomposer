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
#include <QHeaderView>
#include <QTimer>

#include "cvconstants.h"
#include "cvutils.h"
#include "plugwidgets/doubleitemdelegate.h"
#include "plugwidgets/sizewidget.h"


KernelDefinitionWidget::KernelDefinitionWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _table(new QTableWidget(this)),
    _sizeWidget(NULL),
    _layout(new QFormLayout(this))
{
    _layout->setContentsMargins(0, 0, 0, 0);

    Properties sizeProperties;
    sizeProperties.insert("width-minimum", 1);
    sizeProperties.insert("width-maximum", 10);
    sizeProperties.insert("height-minimum", 1);
    sizeProperties.insert("height-maximum", 10);
    _sizeWidget = new SizeWidget(sizeProperties, this);
    connect(_sizeWidget, SIGNAL(valueChanged()), SLOT(onSizeChanged()));
    _layout->addRow("Size :", _sizeWidget);

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

    _layout->addRow("Definition :", _table);
}

QSize KernelDefinitionWidget::sizeHint() const
{
    QSize tableSize = tableSizeHint();

    int width = qMax(_layout->itemAt(0, QFormLayout::LabelRole)->widget()->sizeHint().width(),
                     _layout->itemAt(1, QFormLayout::LabelRole)->widget()->sizeHint().width());
    // FIXME: QFormLayout takes us 6px more that it should, don't know why ?!
    width += 2 * _layout->horizontalSpacing();
    width += qMax(_sizeWidget->sizeHint().width(), tableSize.width());

    int height = _sizeWidget->sizeHint().height() +
                 _layout->verticalSpacing() +
                 tableSize.height();

    return QSize(width, height);
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

    _sizeWidget->blockSignals(true);
    _sizeWidget->setValue(QVariant::fromValue(cv::Size(mat.rows, mat.cols)));
    _sizeWidget->blockSignals(false);

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

QSize KernelDefinitionWidget::tableSizeHint() const
{
    return QSize(_table->horizontalHeader()->length() + 4, _table->verticalHeader()->length() + 4);
}

void KernelDefinitionWidget::onSizeChanged()
{
    cv::Size size = _sizeWidget->getValue().value<cv::Size>();
    _table->setRowCount(size.width);
    _table->setColumnCount(size.height);
    _table->setFixedSize(tableSizeHint());
    _layout->activate();

    emit sizeHintChanged();
}

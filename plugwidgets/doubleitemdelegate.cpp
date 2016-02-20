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

#include "doubleitemdelegate.h"

#include <QDebug>
#include <QDoubleSpinBox>

#include "cvconstants.h"


DoubleItemDelegate::DoubleItemDelegate(const Properties &properties, QObject *parent) :
    QStyledItemDelegate(parent),
    _properties(properties)
{
}

QWidget *DoubleItemDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setProperty("decimals", CvConstants::defaultDoubleDecimals);
    editor->setProperty("maximum", CvConstants::defaultDoubleMax);
    _properties.applyTo(editor);

    connect(editor, SIGNAL(valueChanged(double)), SLOT(onValueChanged()));

    return editor;
}

void DoubleItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->setValue(value);
}

void DoubleItemDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleItemDelegate::updateEditorGeometry(QWidget *editor,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QString DoubleItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return locale.toString(value.toDouble(),
                           'f',
                           _properties.value("decimals",
                                             CvConstants::defaultDoubleDecimals).toInt());
}

void DoubleItemDelegate::onValueChanged()
{
    // We want to update the model as soon as the value has changed, not when editing is finished
    emit commitData(qobject_cast<QWidget *>(sender()));
    emit valueChanged();
}

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

#pragma once

#include <QStyledItemDelegate>

#include "properties.h"

class DoubleItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit DoubleItemDelegate(const Properties &properties, QObject *parent = NULL);

        virtual QWidget *createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const override;

        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;

        virtual void setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const override;

        virtual void updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const override;

        virtual QString displayText(const QVariant & value, const QLocale &locale) const override;

    signals:
        void valueChanged();

    private slots:
        void onValueChanged();

    private:
        const Properties _properties;
};

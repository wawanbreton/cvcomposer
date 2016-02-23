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

#include "plugwidgets/abstractplugwidget.h"

#include <QFormLayout>
#include <QTableWidget>

class SizeWidget;
class EnumerationWidget;

/*! @brief This widget is useful to manually edit the values of a kernel
 *
 *  The following properties may be given :
 *    - *  : all the properties that a QDoubleSpinBox may accept */
class KernelDefinitionWidget : public AbstractPlugWidget
{
    Q_OBJECT

    public:
        explicit KernelDefinitionWidget(const Properties &properties, QWidget *parent = NULL);

        virtual QSize sizeHint() const override;

        virtual QVariant getValue() const override;

        virtual void setValue(const QVariant &value) override;

    private:
        typedef enum
        {
            None,
            Horizontal,
            Vertical,
            Both,
            Center
        } Symmetry;

    private:
        QSize tableSizeHint() const;

        void updateCellColors();

    private slots:
        void onSizeChanged();

        void onSymmetryChanged();

        void onCellEdited(const QModelIndex &index, double value);

    private:
        QTableWidget *_table;
        SizeWidget *_sizeWidget;
        EnumerationWidget *_symmetryWidget;
        QFormLayout *_layout;
};

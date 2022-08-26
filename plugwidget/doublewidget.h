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

#include "plugwidget/abstractplugwidget.h"

#include <QDoubleSpinBox>

#include "global/properties.h"

/*! @brief This widget is useful to edit a decimal value
 *
 *  The following properties may be given :
 *    - *  : all the properties that a QDoubleSpinBox may accept */
class DoubleWidget : public AbstractPlugWidget
{
    Q_OBJECT

    public:
        explicit DoubleWidget(const Properties &properties, QWidget *parent = nullptr);

        virtual QVariant getValue() const override;

        virtual void setValueImpl(const QVariant &value) override;

    private:
        QDoubleSpinBox *const _spinBox;
};

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

#include <QSpinBox>

#include "global/properties.h"

/*! @brief This widget is useful to edit a 2D size
 *
 *  The following properties may be given :
 *    - width-*  : all the properties that a QSpinBox may accept
 *    - height-* : all the properties that a QSpinBox may accept */
class SizeWidget : public AbstractPlugWidget
{
    Q_OBJECT

    public:
        explicit SizeWidget(const Properties &properties, QWidget *parent = NULL);

        virtual QVariant getValue() const override;

        virtual void setValue(const QVariant &value) override;

    private:
        QSpinBox *_spinBoxWidth;
        QSpinBox *_spinBoxHeight;
};

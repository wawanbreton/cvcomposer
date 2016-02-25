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

#ifndef POINTWIDGET_H
#define POINTWIDGET_H

#include "plugwidget/abstractplugwidget.h"

#include <QSpinBox>

#include "global/properties.h"

/*! @brief This widget is useful to edit a 2D point
 *
 *  The following properties may be given :
 *    - x-* : all the properties that a QSpinBox may accept
 *    - y-* : all the properties that a QSpinBox may accept */
class PointWidget : public AbstractPlugWidget
{
    Q_OBJECT

    public:
        explicit PointWidget(const Properties &properties, QWidget *parent = NULL);

        virtual QVariant getValue() const override;

        virtual void setValue(const QVariant &value) override;

    private:
        QSpinBox *_spinBoxX;
        QSpinBox *_spinBoxY;
};

#endif // POINTWIDGET_H

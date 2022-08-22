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

#include <QLineEdit>

/*! @brief This widget is useful to browse to a path
 *
 *  The following properties may be given :
 *    - folder  : a Boolean which indicates wether we want to selected a folder or a file :
 *                defaults to false
 *    - formats : a QList<QPair<QString, QStringList> > which indicates the supported file
 *                formats to be loaded
 */
class PathWidget : public AbstractPlugWidget
{
    Q_OBJECT

    public:
        explicit PathWidget(const Properties &properties, QWidget *parent = NULL);

        virtual QVariant getValue() const override;

        virtual void setValue(const QVariant &value) override;

    private:
        void onButtonPressed();

    private:
        QLineEdit *_lineEdit;
        bool _folder;
        QList<QPair<QString, QStringList> > _formats;
};

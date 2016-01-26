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

#ifndef GENERICNODEWIDGET_H
#define GENERICNODEWIDGET_H

#include <QWidget>

#include <QLabel>

#include "plugdefinition.h"

class GenericNodeWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit GenericNodeWidget(QWidget *parent = NULL);

        void setPlugs(const QList<PlugDefinition> &inputs, const QList<PlugDefinition> &outputs);

        int getPlugPosY(const QString &plugName);

        void setInputPlugged(const QString &inputName, bool plugged);

    private:
        QWidget *makePlugWidget(const PlugDefinition &plug);

    private:
        QMap<QString, QPair<QLabel *, QWidget *> > _widgets;
};

#endif // GENERICNODEWIDGET_H

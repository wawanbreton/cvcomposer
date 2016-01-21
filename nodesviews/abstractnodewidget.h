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

#ifndef ABSTRACTNODEWIDGET_H
#define ABSTRACTNODEWIDGET_H

#include <QWidget>

#include "properties.h"

#include <opencv2/core/core.hpp>

class AbstractNodeWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit AbstractNodeWidget(QWidget *parent = NULL);

        virtual QVariant getProperty(const QString &name) const = 0;

        Properties getProperties() const;

    protected:
        void onPropertyChanged(const QString &name);

        virtual QStringList getPropertiesNames() const = 0;

    protected slots:
        virtual void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        virtual void onProcessUnavailable();

    signals:
        void propertyChanged(const QString &name, const QVariant &value);
};

#endif // ABSTRACTNODEWIDGET_H

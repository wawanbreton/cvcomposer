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

#ifndef BLURWIDGET_H
#define BLURWIDGET_H

#include "nodesviews/abstractnodewidget.h"

#include <opencv2/core/core.hpp>

#include <QMetaType>

namespace Ui { class BlurWidget; }

class BlurWidget : public AbstractNodeWidget
{
    Q_OBJECT

    public:
        explicit BlurWidget(QWidget *parent = NULL);
        BlurWidget(const BlurWidget &other);
        ~BlurWidget();

        virtual QVariant getProperty(const QString &name) const override;

    protected:
        virtual QStringList getPropertiesNames() const override;

    private slots:
        void onSizeChanged();

        void onAnchorChanged();

        void onBorderChanged();

    private:
        Ui::BlurWidget *_ui;
};

Q_DECLARE_METATYPE(BlurWidget)

#endif // BLURWIDGET_H

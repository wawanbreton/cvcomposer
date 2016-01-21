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

#ifndef IMAGEFROMFILEWIDGET_H
#define IMAGEFROMFILEWIDGET_H

#include "nodesviews/abstractnodewidget.h"

namespace Ui { class ImageFromFileWidget; }

class ImageFromFileWidget : public AbstractNodeWidget
{
    Q_OBJECT

    public:
        explicit ImageFromFileWidget(QWidget *parent = NULL);
        ImageFromFileWidget(const ImageFromFileWidget &other);
        ~ImageFromFileWidget();

        virtual QVariant getProperty(const QString &name) const override;

    public slots:
        virtual void setProperty(const QString &name, const QVariant &value) override;

    protected:
        virtual QStringList getPropertiesNames() const override;

    private slots:
        void onEditingFinished();

        void onButtonPressed();

    private:
        Ui::ImageFromFileWidget *_ui;
};

Q_DECLARE_METATYPE(ImageFromFileWidget)

#endif // IMAGEFROMFILEWIDGET_H

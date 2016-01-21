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

#ifndef DOCKABLEIMAGEVIEWERWIDGET_H
#define DOCKABLEIMAGEVIEWERWIDGET_H

#include "nodesviews/abstractnodewidget.h"

namespace Ui { class DockableImageViewerWidget; }

class ImageViewerDockWidget;

class DockableImageViewerWidget : public AbstractNodeWidget
{
    Q_OBJECT

    public:
        explicit DockableImageViewerWidget(QWidget *parent = NULL);
        DockableImageViewerWidget(const DockableImageViewerWidget &other);
        ~DockableImageViewerWidget();

        virtual QVariant getProperty(const QString &name) const override;

    signals:
        void nameChanged(const QString &name);

    protected:
        virtual QStringList getPropertiesNames() const override;

    protected slots:
        virtual void onProcessDone(const QList<cv::Mat> &outputs, const QList<cv::Mat> &inputs);

        virtual void onProcessUnavailable();

    private:
        Ui::DockableImageViewerWidget *_ui;
        ImageViewerDockWidget *_dockWidget;
};

Q_DECLARE_METATYPE(DockableImageViewerWidget)

#endif // DOCKABLEIMAGEVIEWERWIDGET_H

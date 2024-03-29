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

#include <QDockWidget>

#include <opencv2/core/core.hpp>

namespace Ui { class ImageViewerDockWidget; }

class ImageViewerDockWidget : public QDockWidget
{
    Q_OBJECT

    public:
        explicit ImageViewerDockWidget(QWidget *parent = nullptr);
        ~ImageViewerDockWidget();

        void setImage(const QPixmap &image);

    private:
        void onSaveImage();

        void onColorPicked(const QColor &color);

    private:
        static int _count;

    private:
        Ui::ImageViewerDockWidget *_ui;
};

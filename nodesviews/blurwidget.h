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

#include <QWidget>

#include <opencv2/core/core.hpp>

namespace Ui { class BlurWidget; }

class BlurWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit BlurWidget(const cv::Size &size, const cv::Point &anchor, QWidget *parent = 0);
        ~BlurWidget();

    signals:
        void sizeChanged(const cv::Size &size);

        void anchorChanged(const cv::Point &anchor);

    private slots:
        void onSizeChanged();

        void onAnchorChanged();

    private:
        Ui::BlurWidget *_ui;
};

#endif // BLURWIDGET_H

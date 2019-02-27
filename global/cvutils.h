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

#ifndef CVUTILS_H
#define CVUTILS_H

#include <QImage>
#include <opencv2/core/core.hpp>

namespace CvUtils
{
    QImage toQImage(const cv::Mat &mat);

    QList<QPair<QString, QVariant>> makeBlurBorderValues();

    QList<QPair<QString, QVariant>> makeLineTypeValues();

    QList<QPair<QString, QVariant>> makeImageLoadFormatsValues();

    QList<QPair<QString, QStringList>> getImageFormats();

    QString makeFilterFromFileFormats(const QList<QPair<QString, QStringList>> &formats);

    QString makeUrl(const QStringList &parts, const QString &tag = "");
}

Q_DECLARE_METATYPE(cv::Point)
Q_DECLARE_METATYPE(cv::Size)
Q_DECLARE_METATYPE(cv::Rect)
Q_DECLARE_METATYPE(cv::Mat)
Q_DECLARE_METATYPE(cv::Scalar)
Q_DECLARE_METATYPE(cv::RotatedRect)

#endif // CVUTILS_H

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

#include "imageviewerwidget.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>

#include "global/cvutils.h"
#include "plugwidget/imageviewerdockwidget.h"


ImageViewerWidget::ImageViewerWidget(QWidget *parent) :
    AbstractPlugWidget(parent),
    _lineEdit(new QLineEdit(this)),
    _dockWidget(new ImageViewerDockWidget(QApplication::activeWindow()))
{
    // Use a layout so that it manages the size hint and resize event
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_lineEdit);

    QString title = "Image viewer";

    _lineEdit->setText(title);
    connect(_lineEdit,   SIGNAL(textChanged(QString)),
            _dockWidget, SLOT(setWindowTitle(QString)));

    _dockWidget->show();
    _dockWidget->setWindowTitle(title);
}

ImageViewerWidget::~ImageViewerWidget()
{
    delete _dockWidget;
}

QMap<QString, QString> ImageViewerWidget::save() const
{
    QMap<QString, QString> properties;

    properties.insert("title", _lineEdit->text());
    properties.insert("geometry", QString::fromUtf8(_dockWidget->saveGeometry().toHex()));

    // We need to store dock widgets object name so that their state
    // inside the QMainWindow is restored correctly
    properties.insert("objectName", _dockWidget->objectName());

    return properties;
}

void ImageViewerWidget::load(const QMap<QString, QString> &properties)
{
    _lineEdit->setText(properties["title"]);
    _dockWidget->setObjectName(properties["objectName"]);
    _dockWidget->restoreGeometry(QByteArray::fromHex(properties["geometry"].toUtf8()));
}

void ImageViewerWidget::onNodeProcessed(const Properties &inputs, const Properties &outputs)
{
    AbstractPlugWidget::onNodeProcessed(inputs, outputs);

    _dockWidget->setImage(inputs["image"].value<cv::Mat>());
}

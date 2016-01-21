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

#include "dockableimageviewerwidget.h"
#include "ui_dockableimageviewerwidget.h"

#include <QApplication>

#include "nodesviews/imageviewerdockwidget.h"


DockableImageViewerWidget::DockableImageViewerWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::DockableImageViewerWidget),
    _dockWidget(new ImageViewerDockWidget(QApplication::activeWindow()))
{
    _ui->setupUi(this);

    QString title = "Image viewer";

    _ui->lineEdit->setText(title);
    connect(_ui->lineEdit, SIGNAL(textChanged(QString)),
            _dockWidget,   SLOT(setWindowTitle(QString)));

    _dockWidget->show();
    _dockWidget->setWindowTitle(title);
}

DockableImageViewerWidget::DockableImageViewerWidget(const DockableImageViewerWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("DockableImageViewerWidget::DockableImageViewerWidget");
}

DockableImageViewerWidget::~DockableImageViewerWidget()
{
    delete _ui;
}

QStringList DockableImageViewerWidget::getPropertiesNames() const
{
    return QStringList();
}

void DockableImageViewerWidget::setProperty(const QString &name, const QVariant &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
}

QVariant DockableImageViewerWidget::getProperty(const QString &name) const
{
    Q_UNUSED(name);
    return QVariant();
}

void DockableImageViewerWidget::onProcessDone(const QList<cv::Mat> &outputs,
                                              const QList<cv::Mat> &inputs)
{
    Q_UNUSED(outputs); // Previewer has no ouput, it only displays the input image
    _dockWidget->setImage(inputs[0]);
}

void DockableImageViewerWidget::onProcessUnavailable()
{
    _dockWidget->setImage(cv::Mat());
}

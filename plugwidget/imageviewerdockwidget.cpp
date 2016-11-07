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

#include "imageviewerdockwidget.h"
#include "ui_imageviewerdockwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QImageWriter>

#include "global/cvutils.h"


int ImageViewerDockWidget::_count = 0;

ImageViewerDockWidget::ImageViewerDockWidget(QWidget *parent) :
    QDockWidget(parent),
    _ui(new Ui::ImageViewerDockWidget)
{
    _ui->setupUi(this);

    // Saving and restoring dock widgets properly requires all of them to have a unique object name
    setObjectName(QString("ImageViewerDockWidget%1").arg(_count++));

    connect(_ui->graphicsView, SIGNAL(colorPicked(QColor)), SLOT(onColorPicked(QColor)));
    onColorPicked(Qt::white);

    QFontMetrics metrics = fontMetrics();
    int maxColorWidth = metrics.width("888");
    _ui->labelColorBlue->setFixedWidth(maxColorWidth);
    _ui->labelColorGreen->setFixedWidth(maxColorWidth);
    _ui->labelColorRed->setFixedWidth(maxColorWidth);
}

ImageViewerDockWidget::~ImageViewerDockWidget()
{
    delete _ui;
}

void ImageViewerDockWidget::setImage(const QPixmap &image)
{
    _ui->graphicsView->setImage(image);
}

void ImageViewerDockWidget::onSaveImage()
{
    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();

    QList<QPair<QString, QStringList> > mergedFormats;

    for(const QPair<QString, QStringList> &format : CvUtils::getImageFormats())
    {
        QStringList supportedExtensions;
        foreach(const QString &extension, format.second)
        {
            if(supportedFormats.contains(extension.toUtf8()))
            {
                supportedExtensions << extension;
            }
        }

        if(!supportedExtensions.isEmpty())
        {
            mergedFormats << qMakePair(format.first, supportedExtensions);
        }
    }

    QString fileName = QFileDialog::getSaveFileName(QApplication::activeWindow(),
                                                    "Save file",
                                                    "",
                                                    CvUtils::makeFilterFromImageFormats(mergedFormats));
    if(!fileName.isEmpty())
    {
        _ui->graphicsView->getImage().save(fileName);
    }
}

void ImageViewerDockWidget::onColorPicked(const QColor &color)
{
    // RBG
    _ui->labelColorBlue->setText(QString::number(color.blue()));
    _ui->labelColorGreen->setText(QString::number(color.green()));
    _ui->labelColorRed->setText(QString::number(color.red()));

    // HTML
    QString htmlColor = "#";
    htmlColor += QString::number(color.red(), 16).rightJustified(2, '0');
    htmlColor += QString::number(color.green(), 16).rightJustified(2, '0');
    htmlColor += QString::number(color.blue(), 16).rightJustified(2, '0');
    _ui->lineEditColorHexa->setText(htmlColor);

    // Visual
    QPalette palette = _ui->lineEditColor->palette();
    palette.setColor(QPalette::Base, color);
    _ui->lineEditColor->setPalette(palette);
}

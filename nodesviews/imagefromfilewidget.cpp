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

#include "imagefromfilewidget.h"
#include "ui_imagefromfilewidget.h"

#include <QApplication>
#include <QFileDialog>


ImageFromFileWidget::ImageFromFileWidget(QWidget *parent) :
    AbstractNodeWidget(parent),
    _ui(new Ui::ImageFromFileWidget)
{
    _ui->setupUi(this);

    connect(_ui->lineEdit, SIGNAL(editingFinished()), SLOT(onEditingFinished()));
    connect(_ui->buttonBrowse, SIGNAL(clicked(bool)), SLOT(onButtonPressed()));
}

ImageFromFileWidget::ImageFromFileWidget(const ImageFromFileWidget &other) :
    AbstractNodeWidget(other.parentWidget())
{
    qFatal("ImageFromFileWidget::ImageFromFileWidget");
}

ImageFromFileWidget::~ImageFromFileWidget()
{
    delete _ui;
}

QVariant ImageFromFileWidget::getProperty(const QString &name) const
{
    if(name == "path")
    {
        return _ui->lineEdit->text();
    }

    return QVariant();
}

void ImageFromFileWidget::setProperty(const QString &name, const QVariant &value)
{
    if(name == "path")
    {
        _ui->lineEdit->setText(value.toString());
    }
}

QStringList ImageFromFileWidget::getPropertiesNames() const
{
    return QStringList() << "path";
}

void ImageFromFileWidget::onEditingFinished()
{
    onPropertyChanged("path");
}

void ImageFromFileWidget::onButtonPressed()
{
    QList<QPair<QString, QStringList> > formats;
    formats << qMakePair(QString("JPEG"), QStringList() << "jpeg" << "jpg" << "jpe");
    formats << qMakePair(QString("Portable network graphics"), QStringList() << "png");
    formats << qMakePair(QString("TIFF"), QStringList() << "tiff" << "tif");
    formats << qMakePair(QString("JPEG 2000"), QStringList() << "jp2");
    formats << qMakePair(QString("Windows Bitmap"), QStringList() << "bmp");
    formats << qMakePair(QString("OpenEXR"), QStringList() << "exr");
    formats << qMakePair(QString("Sun raster"), QStringList() << "sr" << "ras");
    formats << qMakePair(QString("Portable image formats"), QStringList() << "pbm" << "pgm" << "ppm");

    QStringList allImagesFilter;
    for(int i = 0 ; i < formats.count() ; i++)
    {
        foreach(const QString &format, formats[i].second)
        {
            allImagesFilter << ("*." + format);
        }
    }

    QStringList filters;
    filters << "All supported images (" + allImagesFilter.join(" ") + ")";

    for(int i = 0 ; i < formats.count() ; i++)
    {
        QStringList extensions;
        foreach(const QString &format, formats[i].second)
        {
            extensions << ("*." + format);
        }

        filters << formats[i].first + " (" + extensions.join(" ") + ")";
    }

    QString fileName = QFileDialog::getOpenFileName(QApplication::activeWindow(),
                                                    "Open image file",
                                                    _ui->lineEdit->text(),
                                                    filters.join(";;"));
    if(not fileName.isEmpty())
    {
        _ui->lineEdit->setText(fileName);
        onEditingFinished();
    }
}

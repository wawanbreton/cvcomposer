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

#include "imagepathwidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>


ImagePathWidget::ImagePathWidget(QWidget *parent) :
    AbstractPlugWidget(parent)
{
    setMinimumWidth(350);

    _lineEdit = new QLineEdit(this);
    _lineEdit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QPushButton *buttonBrowse = new QPushButton(this);
    buttonBrowse->setMaximumWidth(40);
    buttonBrowse->setText("...");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(_lineEdit);
    layout->addWidget(buttonBrowse);
    layout->setStretch(0, 1);

    connect(_lineEdit,    SIGNAL(editingFinished()), SIGNAL(valueChanged()));
    connect(buttonBrowse, SIGNAL(clicked(bool)),     SLOT(onButtonPressed()));
}

QVariant ImagePathWidget::getValue() const
{
    return _lineEdit->text();
}

void ImagePathWidget::onButtonPressed()
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
                                                    _lineEdit->text(),
                                                    filters.join(";;"));
    if(not fileName.isEmpty())
    {
        _lineEdit->setText(fileName);
        emit valueChanged();
    }
}

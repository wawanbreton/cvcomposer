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

#include "pathwidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>


PathWidget::PathWidget(const Properties &properties, QWidget *parent) :
    AbstractPlugWidget(parent),
    _lineEdit(new QLineEdit(this)),
    _folder(properties["folder"].toBool()),
    _formats(properties["formats"].value<QList<QPair<QString, QStringList> > >())
{
    setMinimumWidth(350);

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

QVariant PathWidget::getValue() const
{
    return _lineEdit->text();
}

void PathWidget::setValue(const QVariant &value)
{
    _lineEdit->setText(value.toString());
}

void PathWidget::onButtonPressed()
{
    QString path;

    if(_folder)
    {
        path = QFileDialog::getExistingDirectory(QApplication::activeWindow(),
                                                 "Select folder",
                                                 _lineEdit->text());
    }
    else
    {
        QStringList allFileFilter;
        for(int i = 0 ; i < _formats.count() ; i++)
        {
            foreach(const QString &format, _formats[i].second)
            {
                allFileFilter << ("*." + format);
            }
        }

        QStringList filters;
        filters << "All supported files (" + allFileFilter.join(" ") + ")";

        for(int i = 0 ; i < _formats.count() ; i++)
        {
            QStringList extensions;
            foreach(const QString &format, _formats[i].second)
            {
                extensions << ("*." + format);
            }

            filters << _formats[i].first + " (" + extensions.join(" ") + ")";
        }

        path = QFileDialog::getOpenFileName(QApplication::activeWindow(),
                                            "Select file",
                                            _lineEdit->text(),
                                            filters.join(";;"));
    }

    if(not path.isEmpty())
    {
        _lineEdit->setText(path);
        emit valueChanged();
    }
}

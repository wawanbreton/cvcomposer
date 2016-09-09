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

#include "folderpathwidget.h"

#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>


FolderPathWidget::FolderPathWidget(QWidget *parent) :
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


QVariant FolderPathWidget::getValue() const
{
    return _lineEdit->text();
}

void FolderPathWidget::setValue(const QVariant &value)
{
    _lineEdit->setText(value.toString());
}

void FolderPathWidget::onButtonPressed()
{
    QString folderPath = QFileDialog::getExistingDirectory(QApplication::activeWindow(),
                                                           "Images folder",
                                                           _lineEdit->text());
    if(not folderPath.isEmpty())
    {
        _lineEdit->setText(folderPath);
        emit valueChanged();
    }
}

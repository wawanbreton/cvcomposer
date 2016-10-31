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

#include "errordisplaydialog.h"
#include "ui_errordisplaydialog.h"


ErrorDisplayDialog::ErrorDisplayDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ErrorDisplayDialog)
{
    _ui->setupUi(this);
}

void ErrorDisplayDialog::displayError(const QString &error)
{
    ErrorDisplayDialog *dialog = new ErrorDisplayDialog(qApp->activeWindow());
    dialog->_ui->plainTextEdit->setPlainText(error);
    connect(dialog->_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), dialog, SLOT(deleteLater()));
    dialog->show();
}

ErrorDisplayDialog::~ErrorDisplayDialog()
{
    delete _ui;
}

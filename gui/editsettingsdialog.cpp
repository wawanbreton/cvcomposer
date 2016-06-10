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

#include "editsettingsdialog.h"
#include "ui_editsettingsdialog.h"

#include <QDebug>
#include <QMouseEvent>


EditSettingsDialog::EditSettingsDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::EditSettingsDialog)
{
    _ui->setupUi(this);

    _ui->labelCache->installEventFilter(this);
    _ui->labelMultiThreading->installEventFilter(this);
    _ui->labelOptimalThreads->installEventFilter(this);
    _ui->labelFixedThreads->installEventFilter(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    adjustSize();
}

EditSettingsDialog::~EditSettingsDialog()
{
    delete _ui;
}

ExecutorSettings EditSettingsDialog::getSettings() const
{
    ExecutorSettings settings;
    settings.cacheData = _ui->checkBoxCache->isChecked();
    settings.useMultiThreading = _ui->checkBoxMultiThreading->isChecked();
    settings.useOptimalThreadsCount = _ui->radioButtonOptimalThreads->isChecked();
    settings.fixedThreadsCount = _ui->spinBoxFixedThreads->value();

    return settings;
}

void EditSettingsDialog::setSettings(const ExecutorSettings &settings)
{
    _ui->checkBoxCache->setChecked(settings.cacheData);
    _ui->checkBoxMultiThreading->setChecked(settings.useMultiThreading);
    _ui->radioButtonOptimalThreads->setChecked(settings.useOptimalThreadsCount);
    _ui->radioButtonFixedThreads->setChecked(!settings.useOptimalThreadsCount);
    _ui->spinBoxFixedThreads->setValue(settings.fixedThreadsCount);
}

bool EditSettingsDialog::eventFilter(QObject *receiver, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(receiver == _ui->labelCache)
        {
            _ui->checkBoxCache->toggle();
        }
        else if(receiver == _ui->labelMultiThreading)
        {
            _ui->checkBoxMultiThreading->toggle();
        }
        else if(receiver == _ui->labelOptimalThreads)
        {
            _ui->radioButtonOptimalThreads->setChecked(true);
        }
        else if(receiver == _ui->labelFixedThreads)
        {
            _ui->radioButtonFixedThreads->setChecked(true);
        }
        return true;
    }

    return QDialog::eventFilter(receiver, event);
}

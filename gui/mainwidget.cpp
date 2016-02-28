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

#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QFileDialog>

#include "nodestypesmanager.h"


MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWidget),
    _currentFilePath()
{
    _ui->setupUi(this);

    foreach(QTreeWidgetItem *topLevelItem, NodesTypesManager::getTreeItems())
    {
        _ui->treeWidget->addTopLevelItem(topLevelItem);
    }

    updateTitle();

    connect(_ui->actionSave,   SIGNAL(triggered()), SLOT(onSave()));
    connect(_ui->actionSaveAs, SIGNAL(triggered()), SLOT(onSave()));
}

MainWidget::~MainWidget()
{
    delete _ui;
}

void MainWidget::onSave()
{
    if(_currentFilePath.isEmpty() || sender() == _ui->actionSaveAs)
    {
        QString filePath = QFileDialog::getSaveFileName(this, "Save", "", "CvComposer projects (*.cvc)");
        if(filePath.isEmpty())
        {
            return;
        }
        else
        {
            _currentFilePath = filePath;
        }
    }
}

void MainWidget::updateTitle()
{
    setWindowFilePath(_currentFilePath.isEmpty() ? "New project" : _currentFilePath);

    QString title("%1 - %2[*]");
    title = title.arg(QCoreApplication::applicationName());
    setWindowTitle(title.arg(_currentFilePath.isEmpty() ? "New project" : _currentFilePath));
}

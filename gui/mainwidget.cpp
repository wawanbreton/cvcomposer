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

#include <QCloseEvent>
#include <QDebug>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include "execution/composerscheduler.h"
#include "model/composermodel.h"
#include "model/connection.h"
#include "model/node.h"
#include "gui/genericnodeitem.h"
#include "gui/connectionitem.h"
#include "gui/composerscene.h"
#include "gui/editsettingsdialog.h"
#include "gui/processorsitemmodel.h"
#include "gui/processorsmodelfilter.h"


MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWidget),
    _currentFilePath()
{
    _ui->setupUi(this);

    _ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _ui->treeWidget->setDragEnabled(true);
    _ui->treeWidget->setDragDropMode(QAbstractItemView::DragOnly);

    QStandardItemModel *modelProcessors = new ProcessorsItemModel(this);
    QSortFilterProxyModel *filter = new ProcessorsModelFilter(this);
    filter->setSourceModel(modelProcessors);
    filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(_ui->lineEditSearch, &QLineEdit::textChanged,
            filter,              &QSortFilterProxyModel::setFilterFixedString);
    connect(_ui->lineEditSearch, &QLineEdit::textChanged,
            _ui->treeWidget,     &QTreeView::expandAll);
    _ui->treeWidget->setModel(filter);

    _ui->treeWidget->expandAll();

    updateTitle();

    updateRecents();

    connect(_ui->actionNew,      SIGNAL(triggered()), SLOT(onNew()));
    connect(_ui->actionSave,     SIGNAL(triggered()), SLOT(onSave()));
    connect(_ui->actionSaveAs,   SIGNAL(triggered()), SLOT(onSave()));
    connect(_ui->actionLoad,     SIGNAL(triggered()), SLOT(onLoad()));
    connect(_ui->actionSettings, SIGNAL(triggered()), SLOT(onDisplaySettings()));

    // Do not quit automatically, we are going to decide
    qApp->setQuitOnLastWindowClosed(false);
}

MainWidget::~MainWidget()
{
    delete _ui;
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    ComposerScene *scene = qobject_cast<ComposerScene *>(_ui->graphicsView->scene());
    if(scene)
    {
        // The window will be closed right now, but the application may be exited a bit later
        connect(scene, SIGNAL(ended()), qApp, SLOT(quit()));
        scene->end();
    }
    else
    {
        qCritical() << "Scene is not a ComposerScene instance";
        qApp->quit();
    }
}

void MainWidget::onNew()
{
    _ui->graphicsView->replaceScene(new ComposerScene(this));

    _currentFilePath.clear();

    updateTitle();
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

    QDomDocument doc;
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" ");
    doc.appendChild(header);

    ComposerScene *scene = qobject_cast<ComposerScene *>(_ui->graphicsView->scene());
    scene->save(doc, this);

    QFile file(_currentFilePath);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toByteArray(2));
        updateTitle();
        addRecent(_currentFilePath);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to open file for writing");
    }
}

void MainWidget::onLoad()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open", "", "CvComposer projects (*.cvc)");
    if(not filePath.isEmpty())
    {
        loadFile(filePath);
    }
}

void MainWidget::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        QString errorMsg;
        int errorLine, errorColumn;
        if(doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
        {
            _ui->graphicsView->replaceScene(new ComposerScene(doc, this));

            _currentFilePath = filePath;
            updateTitle();
            addRecent(_currentFilePath);
        }
        else
        {
            QString error = "Invalid XML file at line %1 column %2 : %3";
            error = error.arg(errorLine).arg(errorColumn).arg(errorMsg);
            QMessageBox::critical(this, "Error", error);
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to open file");
    }
}

void MainWidget::onLoadRecent()
{
    // Use iconText property instead of text because it contains shortcuts we don't want
    loadFile(sender()->property("iconText").toString());
}

void MainWidget::onDisplaySettings()
{
    EditSettingsDialog *dialog = new EditSettingsDialog(this);
    dialog->setModal(true);

    ComposerScene *scene = qobject_cast<ComposerScene *>(_ui->graphicsView->scene());
    if(scene)
    {
        dialog->setSettings(scene->getScheduler()->getSettings());
    }
    else
    {
        qCritical() << "Scene is not a ComposerScene instance";
    }

    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
    connect(dialog, SIGNAL(accepted()), SLOT(onSettingsAccepted()));

    dialog->show();
}

void MainWidget::onSettingsAccepted()
{
    EditSettingsDialog *dialog = qobject_cast<EditSettingsDialog *>(sender());
    if(dialog)
    {
        ComposerScene *scene = qobject_cast<ComposerScene *>(_ui->graphicsView->scene());
        if(scene)
        {
            scene->accessScheduler()->setSettings(dialog->getSettings());
        }
        else
        {
            qCritical() << "Scene is not a ComposerScene instance";
        }
    }
    else
    {
        qCritical() << "Sender is not an EditSettingsDialog instance";
    }
}

void MainWidget::updateTitle()
{
    QString title("%1 - %2[*]");
    title = title.arg(QCoreApplication::applicationName());
    title = title.arg(_currentFilePath.isEmpty() ? "New project" : QFileInfo(_currentFilePath).fileName());
    setWindowTitle(title);
}

void MainWidget::addRecent(const QString &file)
{
    QSettings settings;
    QStringList recents = settings.value("recent_files").toStringList();

    recents.removeAll(file);
    recents.push_front(file);
    recents = recents.mid(0, 8);

    settings.setValue("recent_files", recents);

    updateRecents(settings);
}

void MainWidget::updateRecents(const QSettings &settings)
{
    _ui->menuRecents->clear();

    bool hasRecents = false;
    foreach(const QString &recent, settings.value("recent_files").toStringList())
    {
        _ui->menuRecents->addAction(recent, this, SLOT(onLoadRecent()));
        hasRecents = true;
    }

    _ui->menuRecents->setEnabled(hasRecents);
}

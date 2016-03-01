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

#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

#include "model/node.h"
#include "gui/genericnodeitem.h"
#include "gui/composerscene.h"
#include "gui/nodestypesmanager.h"


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

    QDomDocument doc;
    QDomProcessingInstruction header = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" ");
    doc.appendChild(header);

    QDomElement rootNode = doc.createElement(QCoreApplication::applicationName().toLower());

    ComposerScene *scene = qobject_cast<ComposerScene *>(_ui->graphicsView->scene());
    foreach(GenericNodeItem *nodeItem ,scene->getNodes())
    {
        const Node *node = nodeItem->getNode();

        QDomElement nodeElement = doc.createElement("node");
        nodeElement.setAttribute("name", node->getName());
        nodeElement.setAttribute("id", QString::number(quint64(node)));

        foreach(const Plug *inputPlug, node->getInputs())
        {
            if(PlugType::isInputPluggable(inputPlug->getDefinition().type) != PlugType::Mandatory)
            {
                QDomElement propertyElement = doc.createElement("property");
                propertyElement.setAttribute("name", inputPlug->getDefinition().name);
                propertyElement.setAttribute("value", inputPlug->save(node->getProperties()[inputPlug->getDefinition().name]));
                nodeElement.appendChild(propertyElement);
            }
        }

        QString itemPos("%1:%2");
        itemPos = itemPos.arg(QString::number(nodeItem->pos().x(), 'f', 2));
        itemPos = itemPos.arg(QString::number(nodeItem->pos().y(), 'f', 2));

        QDomElement itemPropertyElement = doc.createElement("item-property");
        itemPropertyElement.setAttribute("name", "pos");
        itemPropertyElement.setAttribute("value", itemPos);
        nodeElement.appendChild(itemPropertyElement);

        rootNode.appendChild(nodeElement);
    }

    doc.appendChild(rootNode);

    QFile file(_currentFilePath);
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toByteArray(2));
    }
    else
    {
        QMessageBox::critical(this, "Error", "Unable to open file for writing");
    }

    updateTitle();
}

void MainWidget::updateTitle()
{
    QString title("%1 - %2[*]");
    title = title.arg(QCoreApplication::applicationName());
    title = title.arg(_currentFilePath.isEmpty() ? "New project" : QFileInfo(_currentFilePath).fileName());
    setWindowTitle(title);
}

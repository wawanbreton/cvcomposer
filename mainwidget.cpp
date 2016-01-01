#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "nodestypesmanager.h"


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MainWidget)
{
    _ui->setupUi(this);

    foreach(QTreeWidgetItem *topLevelItem, NodesTypesManager::getTreeItems())
    {
        _ui->treeWidget->addTopLevelItem(topLevelItem);
    }
}

MainWidget::~MainWidget()
{
    delete _ui;
}

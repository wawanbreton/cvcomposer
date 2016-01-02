#include "nodestypesmanager.h"

#include <QDebug>

#include "nodes/blurnode.h"


NodesTypesManager *NodesTypesManager::_instance = NULL;

NodesTypesManager::NodesTypesManager()
{
}

NodesTypesManager *NodesTypesManager::get()
{
    if(_instance == NULL)
    {
        _instance = new NodesTypesManager();
    }

    return _instance;
}

QList<QTreeWidgetItem *> NodesTypesManager::getTreeItems()
{
    QList<QTreeWidgetItem *> result;

    QTreeWidgetItem *visualisers = new QTreeWidgetItem(QStringList() << tr("Visualisers"));
    visualisers->setFlags(visualisers->flags() & ~Qt::ItemIsDragEnabled);
    result << visualisers;

    QTreeWidgetItem *filters = new QTreeWidgetItem(QStringList() << tr("Filters"));
    filters->setFlags(visualisers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemBlur = new QTreeWidgetItem(filters, QStringList() << tr("Blur"));
    itemBlur->setData(0, Qt::UserRole, "blur");
    QTreeWidgetItem *itemGaussianBlur = new QTreeWidgetItem(filters, QStringList() << tr("Gaussian blur"));
    itemGaussianBlur->setData(0, Qt::UserRole, "gaussianblur");
    result << filters;

    return result;
}

AbstractNode *NodesTypesManager::createNode(const QString &type)
{
    if(type == "blur")
    {
        return new BlurNode();
    }
    else
    {
        qCritical() << "Unable to instantiate node for type" << type;
        return NULL;
    }
}

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

#include "nodestypesmanager.h"

#include <QDebug>

#include "nodes/gaussianblurnode.h"
#include "nodes/blurnode.h"
#include "nodes/imagefilenode.h"
#include "nodes/imageviewernode.h"


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

    QTreeWidgetItem *inputs = new QTreeWidgetItem(QStringList() << tr("Inputs"));
    inputs->setFlags(inputs->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemFile = new QTreeWidgetItem(inputs, QStringList() << tr("Image from file"));
    itemFile->setData(0, Qt::UserRole, "imagefile");
    result << inputs;

    QTreeWidgetItem *viewers = new QTreeWidgetItem(QStringList() << tr("Viewers"));
    viewers->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemDockableViewer = new QTreeWidgetItem(viewers, QStringList() << tr("Dockable image viewer"));
    itemDockableViewer->setData(0, Qt::UserRole, "dockableimageviewer");
    QTreeWidgetItem *itemPreviewer = new QTreeWidgetItem(viewers, QStringList() << tr("Image previewer"));
    itemPreviewer->setData(0, Qt::UserRole, "imagepreviewer");
    result << viewers;

    QTreeWidgetItem *filters = new QTreeWidgetItem(QStringList() << tr("Filters"));
    filters->setFlags(viewers->flags() & ~Qt::ItemIsDragEnabled);
    QTreeWidgetItem *itemBlur = new QTreeWidgetItem(filters, QStringList() << tr("Blur"));
    itemBlur->setData(0, Qt::UserRole, "blur");
    QTreeWidgetItem *itemGaussianBlur = new QTreeWidgetItem(filters, QStringList() << tr("Gaussian blur"));
    itemGaussianBlur->setData(0, Qt::UserRole, "gaussianblur");
    result << filters;

    return result;
}

AbstractNode *NodesTypesManager::createNode(const QString &type)
{
    if(type == "imagepreviewer")
    {
        return new ImageViewerNode(true);
    }
    else if(type == "dockableimageviewer")
    {
        return new ImageViewerNode(false);
    }
    else if(type == "blur")
    {
        return new BlurNode();
    }
    else if(type == "gaussianblur")
    {
        return new GaussianBlurNode();
    }
    else if(type == "imagefile")
    {
        return new ImageFileNode();
    }
    else
    {
        qCritical() << "Unable to instantiate node for type" << type;
        return NULL;
    }
}

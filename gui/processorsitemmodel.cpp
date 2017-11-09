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

#include "processorsitemmodel.h"

#include <QMimeData>

#include "processor/processorsfactory.h"


ProcessorsItemModel::ProcessorsItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
    QList<QPair<QString, QStringList> > nodes = ProcessorsFactory::getProcessors();
    for(const QPair<QString, QStringList> &group : nodes)
    {
        QStandardItem *groupItem = new QStandardItem(group.first);
        groupItem->setFlags(groupItem->flags() & ~Qt::ItemIsDragEnabled);

        for(const QString &node : group.second)
        {
            QString nodeName = ProcessorsFactory::toUserReadableName(node);
            QStandardItem *itemNode = new QStandardItem(nodeName);
            itemNode->setData(node, Qt::UserRole);
            groupItem->appendRow(itemNode);
        }

        appendRow(groupItem);
    }
}

QMimeData *ProcessorsItemModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *data = new QMimeData();
    data->setData("application/x-cvcomposerfilter",
                  indexes.at(0).data(Qt::UserRole).toString().toUtf8());
    return data;
}

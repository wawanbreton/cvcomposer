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

#include "filterslistwidget.h"

#include <QMimeData>


FiltersListWidget::FiltersListWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

QMimeData *FiltersListWidget::mimeData(const QList<QTreeWidgetItem *> items) const
{
    QMimeData *data = new QMimeData();
    data->setData("application/x-cvcomposerfilter", items.at(0)->data(0, Qt::UserRole).toString().toUtf8());
    return data;
}

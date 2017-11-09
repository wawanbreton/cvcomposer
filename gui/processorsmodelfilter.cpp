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

#include "processorsmodelfilter.h"

#include <QDebug>


ProcessorsModelFilter::ProcessorsModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool ProcessorsModelFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if(!sourceParent.isValid())
    {
        // No parent : this is a group item, display it only if it has some displayed children
        QModelIndex currentIndex = sourceModel()->index(sourceRow, 0);
        bool atLeastOneChildDisplayed = false;
        QModelIndex child = currentIndex.child(0, 0);
        while(child.isValid())
        {
            if(filterAcceptsRow(child.row(), currentIndex))
            {
                atLeastOneChildDisplayed = true;
                break;
            }

            child = currentIndex.child(child.row() + 1, 0);
        }

        return atLeastOneChildDisplayed;
    }
    else
    {
        // This is a processor item, filter it with usual method
        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
}

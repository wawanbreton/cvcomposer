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

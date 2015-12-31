#include "filterslistwidget.h"


FiltersListWidget::FiltersListWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}

QStringList FiltersListWidget::mimeTypes() const
{
    return QStringList() << "application/x-cvcomposerfilter";
}


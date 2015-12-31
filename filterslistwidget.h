#ifndef FILTERSLISTWIDGET_H
#define FILTERSLISTWIDGET_H

#include <QTreeWidget>

class FiltersListWidget : public QTreeWidget
{
    Q_OBJECT

    public:
        explicit FiltersListWidget(QWidget *parent = NULL);

    protected:
        virtual QStringList mimeTypes() const;
};

#endif // FILTERSLISTWIDGET_H

#ifndef NODESTYPESMANAGER_H
#define NODESTYPESMANAGER_H

#include <QObject>

#include <QTreeWidgetItem>

class AbstractNode;

class NodesTypesManager : public QObject
{
    Q_OBJECT

    public:
        NodesTypesManager();

    public:
        static NodesTypesManager *get();

        static QList<QTreeWidgetItem *> getTreeItems();

        static AbstractNode *createNode(const QString &type);

    private:
        static NodesTypesManager *_instance;
};

#endif // NODESTYPESMANAGER_H

#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QObject>

class AbstractNode : public QObject
{
    Q_OBJECT
public:
    explicit AbstractNode(QObject *parent = 0);

signals:

public slots:

};

#endif // ABSTRACTNODE_H

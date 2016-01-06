#ifndef COMPOSERSCENE_H
#define COMPOSERSCENE_H

#include <QGraphicsScene>

#include <QGraphicsSceneDragDropEvent>

class ConnectionItem;

class ComposerScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        explicit ComposerScene(QObject *parent = NULL);

    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        ConnectionItem *_editedConnection;
        bool _editedConnectionFromStart;
};

#endif // COMPOSERSCENE_H

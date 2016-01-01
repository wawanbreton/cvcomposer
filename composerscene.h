#ifndef COMPOSERSCENE_H
#define COMPOSERSCENE_H

#include <QGraphicsScene>

#include <QGraphicsSceneDragDropEvent>

class ComposerScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        explicit ComposerScene(QObject *parent = NULL);

    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
};

#endif // COMPOSERSCENE_H

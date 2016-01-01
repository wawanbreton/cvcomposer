#ifndef COMPOSERWIDGET_H
#define COMPOSERWIDGET_H

#include <QGraphicsView>

#include <QGraphicsScene>

class ComposerWidget : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit ComposerWidget(QWidget *parent = NULL);

    private:
        QGraphicsScene *_scene;
};

#endif // COMPOSERWIDGET_H

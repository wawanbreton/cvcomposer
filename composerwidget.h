#ifndef COMPOSERWIDGET_H
#define COMPOSERWIDGET_H

#include <QGraphicsView>

class ComposerWidget : public QGraphicsView
{
    Q_OBJECT

    public:
        explicit ComposerWidget(QWidget *parent = NULL);

    protected:
        virtual void dragEnterEvent(QDragEnterEvent *event);
};

#endif // COMPOSERWIDGET_H

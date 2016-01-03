#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace Ui { class MainWidget; }

class MainWidget : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = 0);
        ~MainWidget();

    private:
        Ui::MainWidget *_ui;
};

#endif // MAINWIDGET_H

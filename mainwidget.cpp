#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MainWidget)
{
    _ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete _ui;
}

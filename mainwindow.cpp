#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reactor.h"

#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsView* view = new QGraphicsView();
    scene = new QGraphicsScene;
    view->setScene(scene);
    setCentralWidget(view);

    Reactor* reactor = new Reactor();
    scene->addItem(reactor);
    ui->statusbar->addWidget(reactor->d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

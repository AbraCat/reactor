#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reactor.h"

#include <QGraphicsView>
#include <QTimer>

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

    energyGraph = new PlaneItem(1, {Qt::black}, 0.01, 1000, {360, 355, 0}, {360 + 350, 5, 0});
    scene->addItem(energyGraph);

    countGraph = new PlaneItem(2, {Qt::blue, Qt::red}, 0.5, 100, {360, -5, 0}, {360 + 350, -355, 0});
    scene->addItem(countGraph);

    QObject::connect(reactor, &Reactor::energySig, energyGraph, &PlaneItem::addPoint);
    QObject::connect(reactor, &Reactor::molCntSig, countGraph, &PlaneItem::addPoint);

    ui->statusbar->addWidget(reactor->d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

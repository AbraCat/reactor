#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reactor.h"

#include <QGraphicsView>
#include <QTimer>

const int edge = 250;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsView* view = new QGraphicsView();
    scene = new QGraphicsScene;
    view->setScene(scene);
    setCentralWidget(view);

    Reactor* reactor = new Reactor(edge);
    scene->addItem(reactor);

    energyGraph = new PlaneItem(1, {Qt::black}, 0.1, 100, {edge + 5, edge, 0}, {2 * edge + 5, 5, 0});
    countGraph = new PlaneItem(2, {Qt::blue, Qt::red}, 0.4, 100, {edge + 5, 0, 0}, {2 * edge + 5, -edge, 0});
    scene->addItem(energyGraph);
    scene->addItem(countGraph);

    QObject::connect(reactor, &Reactor::energySig, energyGraph, &PlaneItem::addPoint);
    QObject::connect(reactor, &Reactor::molCntSig, countGraph, &PlaneItem::addPoint);

    ui->statusbar->addWidget(reactor->d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

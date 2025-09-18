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

    // Reactor* reactor = new Reactor();
    // scene->addItem(reactor);

    countGraph = new PlaneItem(1, std::vector<QColor>{Qt::black}, 100, {-300, 300, 0}, {0, 0, 0});
    scene->addItem(countGraph);

    QTimer *timer = new QTimer();
    timer->setInterval(1000 / 20);

    PlaneItem* g = countGraph;
    double *cnt = new double;
    *cnt = 0;
    QObject::connect(timer, &QTimer::timeout, g, [g, cnt]{
        g->addPoint({std::sin(*cnt)});
            *cnt += 0.1;
    });
    timer->start();

    // ui->statusbar->addWidget(reactor->d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

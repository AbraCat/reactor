#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "planeitem.h"

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    PlaneItem *energyGraph, *countGraph;
};
#endif // MAINWINDOW_H

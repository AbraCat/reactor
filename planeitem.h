#ifndef PLANEITEM_H
#define PLANEITEM_H

#include "myvector.h"

#include <QGraphicsObject>

class PlaneItem : public QGraphicsObject
{
    Q_OBJECT
public:
    PlaneItem(int nGraphs, std::vector<QColor> colors, int yScale, IntVector TL, IntVector BR);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    IntVector planeToObjectCoord(Vector coord);
    Vector objectToPlaneCoord(IntVector coord);

    bool inRect(IntVector point);

public slots:
    void addPoint(std::vector<double> point);

private:
    IntVector lftUp, rgtDown, centre;
    int xScale, yScale, width, height;

    int nGraphs, nPoints;
    std::vector<std::vector<double>> points;
    std::vector<QColor> colors;
};

#endif // PLANEITEM_H

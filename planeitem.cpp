#include "planeitem.h"

#include <QPainter>

const int axisWidth = 3, cutsWidth = 1, borderWidth = 1, cutsLength = 5, pointSize = 3, vectorWidth = 3;
const double graphStep = 0.2, arrowCoeff = 0.1;

const int stdScale = 10;

PlaneItem::PlaneItem(int nGraphs, std::vector<QColor> colors, int yScale, IntVector TL, IntVector BR)
{
    this->lftUp = TL;
    this->rgtDown = BR;
    this->width = BR.x - TL.x;
    this->height = TL.y - BR.y;
    this->centre = IntVector(-0.4 * width, -0.4 * height, 0);

    this->nGraphs = nGraphs;
    this->nPoints = 10;
    this->colors = colors;
    this->points = std::vector<std::vector<double>>(nGraphs, std::vector<double>(nPoints, 0));

    this->xScale = width * 1.0 / (nPoints + 1);
    this->yScale = yScale;
}

QRectF PlaneItem::boundingRect() const
{
    return QRectF(-width / 2 - 1, -height / 2 - 1, width + 2, height + 2);
}

void PlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(QPen(QBrush(Qt::black), axisWidth));
    painter->setBrush(Qt::transparent);

    painter->drawLine(-width / 2, -centre.y, width / 2, -centre.y);
    painter->drawLine(centre.x, height / 2, centre.x, -height / 2);

    painter->setPen(QPen(QBrush(Qt::black), borderWidth));
    painter->drawRect(-width / 2, -height / 2, width, height);

    painter->setPen(QPen(QBrush(Qt::black), cutsWidth));
    double xFirstCut = std::ceil(objectToPlaneCoord(IntVector(-width / 2, 0, 0)).x);
    for(int x = planeToObjectCoord({xFirstCut, 0, 0}).x; x < width / 2; x += xScale)
        painter->drawLine(x, -centre.y - cutsLength, x, -centre.y + cutsLength);

    double yFirstCut = std::ceil(objectToPlaneCoord(IntVector(0, -height / 2, 0)).y);
    for(int y = planeToObjectCoord({0, yFirstCut, 0}).y; y < height / 2; y += yScale)
        painter->drawLine(centre.x - cutsLength, -y, centre.x + cutsLength, -y);

    for (int nGraph = 0; nGraph < nGraphs; ++nGraph)
    {
        painter->setPen(QPen(QBrush(colors[nGraph]), pointSize));
        for (int i = 0; i < nPoints; ++i)
        {
            IntVector objectPoint = planeToObjectCoord(Vector(i, points[nGraph][i], 0));
            if (inRect(objectPoint))
                painter->drawPoint(objectPoint.x, -objectPoint.y);
        }
    }

    painter->restore();
}

bool PlaneItem::inRect(IntVector point)
{
    return point.x > -width / 2 && point.x < width / 2 && point.y > -height / 2 && point.y < height / 2;
}

IntVector PlaneItem::planeToObjectCoord(Vector coord)
{
    IntVector objectCoord;
    objectCoord.x = coord.x * xScale + centre.x;
    objectCoord.y = coord.y * yScale + centre.y;
    return objectCoord;
}

Vector PlaneItem::objectToPlaneCoord(IntVector coord)
{
    Vector planeCoord;
    planeCoord.x = (coord.x - centre.x) * 1.0 / xScale;
    planeCoord.y = (coord.y - centre.y) * 1.0 / yScale;
    return planeCoord;
}

void PlaneItem::addPoint(std::vector<double> point)
{
    for (int nGraph = 0; nGraph < nGraphs; ++nGraph)
    {
        points[nGraph].push_back(point[nGraph]);
        points[nGraph].erase(points[nGraph].begin());
    }
    update();
}

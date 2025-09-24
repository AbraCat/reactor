#ifndef REACTOR_H
#define REACTOR_H

#include "myvector.h"

#include <QGraphicsObject>
#include <QLabel>
#include <qwidget.h>
#include <QGraphicsSceneMouseEvent>

enum MolType
{
    MOL_ROUND,
    MOL_SQUARE
};

enum MolStatus
{
    MOL_VALID,
    MOL_INVALID,
    MOL_WALL_BOUNCE
};

class Molecule;
class RoundMol;
class SquareMol;
class Reactor;

class Molecule
{
public:
    MolStatus status;
    MolType type;
    int mass, r;
    Vector v, pos;

    Molecule(int mass, Vector v, Vector pos, MolType type);
    virtual ~Molecule() = default;

    virtual void collide(std::vector<Molecule*>& mols, Vector collidePos, Molecule* other) = 0;
    virtual void draw(QPainter* painter) = 0;
};

class RoundMol : public Molecule
{
public:
    RoundMol(int mass, Vector v, Vector pos);

    virtual void collide(std::vector<Molecule*>& mols, Vector collidePos, Molecule* other);
    virtual void draw(QPainter* painter);
};

class SquareMol : public Molecule
{
public:
    SquareMol(int mass, Vector v, Vector pos);

    virtual void collide(std::vector<Molecule*>& mols, Vector collidePos, Molecule* other);
    virtual void draw(QPainter* painter);
};

class Button : public QObject
{
    Q_OBJECT
public:
    Button(int xl, int yt, int xr, int yb, Vector color);
    virtual void action();
    void unpress();

    IntVector TL, BR;
    Vector press_color, unpress_color;
    bool is_pressed;
signals:
    void pressed();
};

// class MoveButton : public Button
// {
//     Q_OBJECT
// public:
//     virtual void action() override;
// };

class Reactor : public QGraphicsObject
{
    Q_OBJECT;
public:
    Reactor(int width);
    ~Reactor();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    double energy();
    std::vector<double> molCnt();

    void checkWallCollision(Molecule* mol);
    void checkMolCollision(Molecule* mol, Molecule* mol2);

    void moveWall(int step);
    void increaseTemp(double step);
    void addRandomMols(int nMols);

    void addButton(Vector color);

signals:
    void energySig(std::vector<double> enegry);
    void molCntSig(std::vector<double> cnt);

public slots:
    void advance();

private:
    // int width, height;
    IntVector TL, BR;
    std::vector<Molecule*> mols;
    QTimer* timer;

    std::vector<Button*> buttons;
    double lftTemp, rgtImpulse;

public:
    QLabel* d;
};

#endif // REACTOR_H

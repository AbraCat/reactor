#ifndef REACTOR_H
#define REACTOR_H

#include "myvector.h"

#include <QGraphicsObject>
#include <QLabel>

enum MolType
{
    MOL_ROUND,
    MOL_SQUARE
};

class Molecule;
class RoundMol;
class SquareMol;
class Reactor;

class Molecule
{
public:
    bool valid;
    MolType type;
    double mass, r;
    Vector v, pos;

    Molecule(double mass, double r, Vector v, Vector pos, MolType type);
    virtual ~Molecule() = default;

    virtual void collide(std::vector<Molecule*>& mols, Molecule* other) = 0;
    virtual void draw(QPainter* painter) = 0;
};

class RoundMol : public Molecule
{
public:
    RoundMol(double mass, double r, Vector v, Vector pos);

    virtual void collide(std::vector<Molecule*>& mols, Molecule* other);
    virtual void draw(QPainter* painter);
};

class SquareMol : public Molecule
{
public:
    SquareMol(double mass, double r, Vector v, Vector pos);

    virtual void collide(std::vector<Molecule*>& mols, Molecule* other);
    virtual void draw(QPainter* painter);
};

class Reactor : public QGraphicsObject
{
    Q_OBJECT;
public:
    QLabel* d;
    Reactor();
    ~Reactor();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public slots:
    void advance();

private:
    int width, height;
    double dt;
    std::vector<Molecule*> mols;

    QTimer* timer;
};

#endif // REACTOR_H

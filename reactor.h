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

class Reactor : public QGraphicsObject
{
    Q_OBJECT;
public:
    Reactor(int width);
    ~Reactor();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    double energy();
    std::vector<double> molCnt();

    void checkWallCollision(Molecule* mol);
    void checkMolCollision(Molecule* mol, Molecule* mol2);

signals:
    void energySig(std::vector<double> enegry);
    void molCntSig(std::vector<double> cnt);

public slots:
    void advance();

private:
    int width, height;
    std::vector<Molecule*> mols;
    QTimer* timer;

public:
    QLabel* d;
};

#endif // REACTOR_H

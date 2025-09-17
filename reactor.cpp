#include "reactor.h"

#include <QPainter>
#include <QTimer>

void printMols(std::vector<Molecule*> mols)
{
    printf("mols dump:\n");
    for (Molecule* mol: mols)
    {
        printf("pos: %.2lf %.2lf type: %d v: %.2lf %.2lf valid: %d\n",
        mol->pos.x, mol->pos.y, mol->type, mol->v.x, mol->v.y, mol->valid);
    }
    printf("mols dump end\n");
}

Molecule::Molecule(double mass, double r, Vector v, Vector pos, MolType type)
{
    this->valid = 1;

    this->mass = mass;
    this->r = r;
    this->v = v;
    this->pos = pos;
    this->type = type;
}

RoundMol::RoundMol(double mass, double r, Vector v, Vector pos) : Molecule(mass, r, v, pos, MOL_ROUND) {};
SquareMol::SquareMol(double mass, double r, Vector v, Vector pos) : Molecule(mass, r, v, pos, MOL_SQUARE) {};

void RoundMol::collide(std::vector<Molecule*>& mols, Molecule* other)
{
    switch (other->type)
    {
        case MOL_ROUND:
        {
            Vector newV = (this->v * this->mass + other->v * other->mass) / (this->mass + other->mass);
            SquareMol* newMol = new SquareMol(this->mass + other->mass, 5, newV, this->pos);
            mols.push_back(newMol);
            break;
        }
        case MOL_SQUARE:
            Vector newV = (this->v * this->mass + other->v * other->mass) / (this->mass + other->mass);
            SquareMol* newMol = new SquareMol(this->mass + other->mass, 5, newV, this->pos);
            mols.push_back(newMol);
            break;
    }
}

void SquareMol::collide(std::vector<Molecule*>& mols, Molecule* other)
{
    switch (other->type)
    {
        case MOL_ROUND:
        {
            other->collide(mols, this);
            break;
        }
        case MOL_SQUARE:
        {
            //
            break;
        }
    }
}

void RoundMol::draw(QPainter* painter)
{
    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPointF(pos.x, -pos.y), r, r);
}

void SquareMol::draw(QPainter* painter)
{
    painter->setBrush(Qt::red);
    painter->drawRect(pos.x - r, -(pos.y - r), r * 2, r * 2);
}

Reactor::Reactor()
{
    width = 200;
    height = 200;
    dt = 1;

    mols = std::vector<Molecule*>();
    mols.reserve(100);

    // mols.push_back(new RoundMol(1, 5, {5, 0, 0}, {0, 0, 0}));
    // mols.push_back(new RoundMol(1, 5, {-5, 0, 0}, {12, 0, 0}));

    for (int i = 0; i < 30; ++i)
    {
        Molecule *mol = nullptr;
        if (rand() % 2) mol = new RoundMol(rand() % 5, 5, {double(rand() % 10 - 5), double(rand() % 10 - 5), 0},
                        {double(rand() % 200 - 100), double(rand() % 200 - 100), 0});
        else mol = new SquareMol(rand() % 5, 5, {double(rand() % 10 - 5), double(rand() % 10 - 5), 0},
                        {double(rand() % 200 - 100), double(rand() % 200 - 100), 0});

        mols.push_back(mol);
    }

    timer = new QTimer();
    timer->setInterval(1000.0 / 20);
    QObject::connect(timer, &QTimer::timeout, this, &Reactor::advance);
    timer->start();

    // QTimer::singleShot(1000, [this]{ advance(); });

    d = new QLabel();
    d->setText("constructor end");
}

Reactor::~Reactor()
{
    for (Molecule* mol: mols)
        delete mol;
    delete timer;
}

QRectF Reactor::boundingRect() const
{
    return QRectF(-width - 1, -height - 1, 2 * (width + 1), 2 * (height + 1));
}

void Reactor::advance()
{
    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != mols.end(); molIter++)
    {
        Molecule* mol = *molIter;
        mol->pos += mol->v * dt;
    }

    std::vector<Molecule*>::iterator molsEnd = mols.end();
    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != molsEnd; molIter++)
    {
        Molecule* mol = *molIter;
        if (mol->valid == 0) continue;

        if (mol->pos.x > width)
        {
            mol->v.x *= -1;
            break;
        }
        if (mol->pos.x < -width)
        {
            mol->v.x *= -1;
            break;
        }
        if (mol->pos.y > height)
        {
            mol->v.y *= -1;
            break;
        }
        if (mol->pos.y < -height)
        {
            mol->v.y *= -1;
            break;
        }

        for (std::vector<Molecule*>::iterator molIter2 = mols.begin(); molIter2 != molsEnd; molIter2++)
        {
            if (molIter == molIter2) continue;
            Molecule* mol2 = *molIter2;
            if (mol2->valid == 0) continue;

            if (*(mol->pos - mol2->pos) < mol->r + mol2->r)
            {
                mol->valid = mol2->valid = 0;
                mol->collide(mols, mol2);
                break;
            }
        }
    }

    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != mols.end(); molIter++)
    {
        if ((*molIter)->valid == 0)
        {
            delete *molIter;
            mols.erase(molIter);
            --molIter;
        }
    }

    update();
}

void Reactor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black, 3));
    painter->setBrush(Qt::transparent);
    painter->drawRect(-width, -height, width * 2, height * 2);
    painter->setPen(QPen(Qt::transparent, 0));

    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != mols.end(); molIter++)
    {
        Molecule* mol = *molIter;
        mol->draw(painter);
    }
}

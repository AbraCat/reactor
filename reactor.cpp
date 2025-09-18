#include "reactor.h"

#include <QPainter>
#include <QTimer>

const double Pi = 3.1415926;

bool isZero(double a)
{
    const double eps = 1e-3;
    return a > -eps && a < eps;
}

void solveQuadratic(double a, double b, double c, double* x1, double* x2, int* nRoots)
{
    double d = b * b - 4 * a * c;
    if (isZero(d))
    {
        *nRoots = 1;
        *x1 = -b / (2 * a);
        *x2 = 0;
        return;
    }
    if (d < 0)
    {
        *nRoots = 0;
        *x1 = *x2 = 0;
        return;
    }
    *nRoots = 2;
    *x1 = (-b - std::sqrt(d)) / (2 * a);
    *x2 = (-b + std::sqrt(d)) / (2 * a);
}

void printMols(std::vector<Molecule*> mols)
{
    printf("mols dump:\n");
    for (Molecule* mol: mols)
    {
        printf("pos: %.2lf %.2lf type: %d v: %.2lf %.2lf status: %d\n",
        mol->pos.x, mol->pos.y, mol->type, mol->v.x, mol->v.y, mol->status);
    }
    printf("mols dump end\n");
}

Molecule::Molecule(int mass, int r, Vector v, Vector pos, MolType type)
{
    this->status = MOL_VALID;

    this->mass = mass;
    this->r = 3.0 * std::sqrt(mass);
    this->v = v;
    this->pos = pos;
    this->type = type;
}

RoundMol::RoundMol(int mass, int r, Vector v, Vector pos) : Molecule(mass, r, v, pos, MOL_ROUND) {};
SquareMol::SquareMol(int mass, int r, Vector v, Vector pos) : Molecule(mass, r, v, pos, MOL_SQUARE) {};

void RoundMol::collide(std::vector<Molecule*>& mols, Vector collidePos, Molecule* other)
{
    switch (other->type)
    {
        case MOL_ROUND:
        {
            Vector newV = (this->v * this->mass + other->v * other->mass) / (this->mass + other->mass);
            SquareMol* newMol = new SquareMol(this->mass + other->mass, 5, newV, collidePos);
            mols.push_back(newMol);
            break;
        }
        case MOL_SQUARE:
            Vector newV = (this->v * this->mass + other->v * other->mass) / (this->mass + other->mass);
            SquareMol* newMol = new SquareMol(this->mass + other->mass, 5, newV, collidePos);
            mols.push_back(newMol);
            break;
    }
}

void SquareMol::collide(std::vector<Molecule*>& mols, Vector collidePos, Molecule* other)
{
    switch (other->type)
    {
        case MOL_ROUND:
        {
            other->collide(mols, pos, this);
            break;
        }
        case MOL_SQUARE:
        {
            const double explodeDT = 0.1;
            // n = (m1+m2)/m0
            // m1*v1+m2*v1=n*m0*v
            // v = (m1v1+m2v2)/(n*m0) = (m1v1+m2v2)/(m1+m2)

            int n = this->mass + other->mass;
            std::vector<Molecule*> newMols(n);
            double angle0 = (rand() % 100) / (100.0 / (2 * Pi));
            double vMod = (rand() % 100) / 20.0 + 5.0;
            Vector vImpulse = (this->v * this->mass + other->v * other->mass) / n;

            for (int i = 0; i < n; ++i)
            {
                double angle = angle0 + i * (2 * Pi / n);
                Vector newV = Vector(vMod * std::cos(angle), vMod * std::sin(angle), 0) + vImpulse;
                mols.push_back(new RoundMol(1, 5, newV, collidePos + newV * explodeDT));
            }

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
    painter->drawRect(pos.x - r, -pos.y - r, r * 2, r * 2);
}

Reactor::Reactor()
{
    width = 350;
    height = 350;
    dt = 1;

    mols = std::vector<Molecule*>();
    mols.reserve(1000);

    for (int i = 0; i < 300; ++i)
    {
        Molecule *mol = nullptr;
        if (rand() % 2) mol = new RoundMol(rand() % 5 + 1, 5, {double(rand() % 10 - 5), double(rand() % 10 - 5), 0},
                        {double(rand() % 200 - 100), double(rand() % 200 - 100), 0});
        else mol = new SquareMol(rand() % 5 + 1, 5, {double(rand() % 10 - 5), double(rand() % 10 - 5), 0},
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
    return QRectF(-width - 100, -height - 100, 2 * (width + 100), 2 * (height + 100));
}

void Reactor::advance()
{
    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != mols.end(); molIter++)
    {
        Molecule* mol = *molIter;
        if (mol->status != MOL_VALID) printf("Wrong molecule status\n");
    }

    std::vector<Molecule*>::iterator molsEnd = mols.end();
    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != molsEnd; molIter++)
    {
        Molecule* mol = *molIter;
        if (mol->status != MOL_VALID) continue;

        Vector newPos = mol->pos + mol->v * dt;
        mol->status = MOL_WALL_BOUNCE;
        if (newPos.x > width)
        {
            mol->pos = Vector(2 * width - newPos.x, newPos.y, 0);
            mol->v.x *= -1;
        }
        else if (newPos.x < -width)
        {
            mol->pos = Vector(-2 * width - newPos.x, newPos.y, 0);
            mol->v.x *= -1;
        }
        else if (newPos.y > height)
        {
            mol->pos = Vector(newPos.x, 2 * height - newPos.y, 0);
            mol->v.y *= -1;
        }
        else if (newPos.y < -height)
        {
            mol->pos = Vector(newPos.x, -2 * height - newPos.y, 0);
            mol->v.y *= -1;
        }
        else
        {
            mol->status = MOL_VALID;
        }

        for (std::vector<Molecule*>::iterator molIter2 = mols.begin(); molIter2 != molsEnd; molIter2++)
        {
            if (molIter == molIter2) continue;
            Molecule* mol2 = *molIter2;
            if (mol2->status != MOL_VALID) continue;

            Vector V = mol->v - mol2->v, P = mol->pos - mol2->pos;
            double R = mol->r + mol2->r;
            double t1 = 0, t2 = 0;
            int nRoots = 0;
            solveQuadratic(V.x * V.x + V.y * V.y, 2 * (P.x * V.x + P.y * V.y), P.x * P.x + P.y * P.y - R * R,
                           &t1, &t2, &nRoots);

            if (nRoots != 2) continue;
            if (t1 < 0 || t1 > dt) continue;

            mol->status = mol2->status = MOL_INVALID;
            Vector critPos1 = mol->pos + mol->v * t1, critPos2 = mol2->pos + mol2->v * t1;
            Vector collidePos = (critPos1 * mol2->r + critPos2 * mol->r) / (mol->r + mol2->r);
            mol->collide(mols, collidePos, mol2);
            break;
        }

        if (mol->status == MOL_VALID) mol->pos = newPos;
    }

    for (std::vector<Molecule*>::iterator molIter = mols.begin(); molIter != mols.end(); molIter++)
    {
        switch ((*molIter)->status)
        {
            case MOL_VALID:
                break;
            case MOL_INVALID:
                delete *molIter;
                mols.erase(molIter);
                --molIter;
                break;
            case MOL_WALL_BOUNCE:
                (*molIter)->status = MOL_VALID;
                break;
        }
    }

    update();
    emit energySig({energy()});
    emit molCntSig(molCnt());
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

double Reactor::energy()
{
    double ans = 0;
    for (Molecule* mol: mols)
        ans += mol->mass * std::pow(*(mol->v), 2) / 2;
    return ans;
}

std::vector<double> Reactor::molCnt()
{
    int round = 0, square = 0;
    for (Molecule* mol: mols)
    {
        if (mol->type == MOL_ROUND) ++round;
        else ++square;
    }

    return {double(round), double(square)};
}

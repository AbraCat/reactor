#ifndef MYVECTOR_H
#define MYVECTOR_H

class IntVector
{
public:
    int x, y, z;
};

class Vector
{
public:
    Vector();
    Vector(double x, double y, double z);

    void print();

    double x, y, z;
};

class FixedVector
{
public:
    Vector p1, p2;
};

Vector operator+(Vector v1, Vector v2);
Vector operator-(Vector v1, Vector v2);
Vector operator*(Vector v1, Vector v2);

Vector& operator+=(Vector& v1, Vector v2);
Vector& operator-=(Vector& v1, Vector v2);
Vector& operator*=(Vector& v, double a);
Vector& operator/=(Vector& v, double a);

Vector operator*(Vector v, double a);
Vector operator/(Vector v, double a);

double operator*(Vector v);
Vector operator!(Vector v);
double operator^(Vector a, Vector b);

Vector proj(Vector a, Vector n);
Vector ortog(Vector a, Vector n);
double dist(Vector p, Vector a, Vector n);

Vector fixedToFree(FixedVector v);
FixedVector freeToFixed(Vector v, Vector start);
FixedVector rotateV(FixedVector v, double angle);
Vector limitVector(Vector v, double lower, double upper);

#endif // MYVECTOR_H

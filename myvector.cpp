#include "myvector.h"

#include <cmath>
#include <cstdio>

Vector fixedToFree(FixedVector v) { return v.p2 - v.p1; }
FixedVector freeToFixed(Vector v, Vector start) { return {start, start + v}; }

IntVector::IntVector() { this->x = this->y = this->z = 0; };
IntVector::IntVector(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector() { this->x = this->y = this->z = 0; };
Vector::Vector(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector operator+(Vector v1, Vector v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
Vector operator-(Vector v1, Vector v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
Vector operator*(Vector v1, Vector v2) { return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}; }

Vector& operator+=(Vector& v1, Vector v2) { v1 = v1 + v2; return v1; }
Vector& operator-=(Vector& v1, Vector v2) { v1 = v1 - v2; return v1; }
Vector& operator*=(Vector& v, double a) { v = v * a; return v; }
Vector& operator/=(Vector& v, double a) { v = v / a; return v; }

Vector operator*(Vector v, double a) { return {v.x * a, v.y * a, v.z * a}; }
Vector operator/(Vector v, double a) { return {v.x / a, v.y / a, v.z / a}; }

double operator*(Vector v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
Vector operator!(Vector v) { return v / (*v); }
double operator^(Vector a, Vector b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

void Vector::print() { printf("vector %.3lf %.3lf %.3lf\n", x, y, z); }

Vector proj(Vector a, Vector n)
{
    double t = (a ^ n) / std::pow(*n, 2);
    return n * t;
}

Vector ortog(Vector a, Vector n)
{
    // (a - tn, n) = 0
    // (a, n) - t|n|^2 = 0
    // t = (a, n) / |n|^2
    double t = (a ^ n) / std::pow(*n, 2);
    return a - n * t;
}

double dist(Vector p, Vector a, Vector n)
{
    // distance from point p to line {a + nt | t in R}
    Vector AP = p - a;
    // AP.print();
    Vector projAP_n = proj(AP, n);
    // projAP_n.print();
    return *(AP - projAP_n);
}

FixedVector rotateV(FixedVector v, double angle)
{
    Vector pAdjust = v.p2 - v.p1;

    double sinA = std::sin(angle), cosA = std::cos(angle);
    Vector newAdjust = {pAdjust.x * cosA - pAdjust.y * sinA, pAdjust.x * sinA + pAdjust.y * cosA, pAdjust.z};
    return {v.p1, v.p1 + newAdjust};
}

Vector limitVector(Vector v, double lower, double upper)
{
    if (v.x < lower) v.x = lower;
    if (v.y < lower) v.y = lower;
    if (v.z < lower) v.z = lower;

    if (v.x > upper) v.x = upper;
    if (v.y > upper) v.y = upper;
    if (v.z > upper) v.z = upper;

    return v;
}

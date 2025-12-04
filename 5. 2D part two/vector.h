#ifndef VECTOR_H
#define VECTOR_H
#include <QPointF>
class Vector
{
    double x,y;
public:
    Vector();
    Vector(double,double);
    Vector(QPointF, QPointF);
    Vector(const Vector&);

    double getX() const;
    double getY() const;
    double getModule() const;
    double operator()();

    int getOctant() const;

    void normalize();
    Vector normalized() const;

    Vector& operator=(const Vector&);

    Vector operator-() const;

    Vector operator+(const Vector&) const;
    Vector operator*(double) const;
    friend Vector operator*(double, const Vector&);
    double operator*(const Vector&) const; // scalar mult
    double operator^(const Vector&) const; // vector mult

    Vector operator/(double) const;
    Vector operator-(const Vector&) const;
    Vector& operator+=(const Vector&);
    Vector& operator-=(const Vector&);
    Vector& operator/=(double);

    Vector& operator*=(double);

    bool operator ==(const Vector&) const;
    bool operator !=(const Vector&) const;

    friend double scalarMult(const Vector& a, const Vector& b);
    friend double vectorMult(const Vector& a, const Vector& b);
};

#endif // VECTOR_H

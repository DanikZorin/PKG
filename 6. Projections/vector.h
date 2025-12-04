#ifndef VECTOR_H
#define VECTOR_H
#include <matrix.h>
class Vector
{
    double x,y,z;
public:
    Vector();
    Vector(double,double,double = 0);
    Vector(const matrix<3,1, double>&);
    Vector(const Vector&);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getModule() const;
    double operator()();

    void normalize();
    Vector normalized() const;

    Vector transformed(matrix<3,3, double> F) const;
    void transform(matrix<3,3, double> F);

    Vector& operator=(const Vector&);

    Vector operator-() const;

    Vector operator+(const Vector&) const;
    Vector operator*(double) const;
    friend Vector operator*(double, const Vector&);
    double operator*(const Vector&) const; // scalar mult
    Vector operator^(const Vector&) const; // vector mult

    Vector operator/(double) const;
    Vector operator-(const Vector&) const;
    Vector& operator+=(const Vector&);
    Vector& operator-=(const Vector&);
    Vector& operator/=(double);

    Vector& operator*=(double);
    Vector& operator^=(const Vector&);

    bool operator ==(const Vector&) const;
    bool operator !=(const Vector&) const;

    friend double scalarMult(const Vector& a, const Vector& b);
    friend Vector VectorMult(const Vector& a, const Vector& b);
};

#endif // VECTOR_H

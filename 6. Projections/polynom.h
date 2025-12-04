#ifndef POLYNOM_H
#define POLYNOM_H
#include <vector>
#include <stdexcept>
class Polynom
{
protected:
    std::vector<double> koefs;
    double getKoef(double t) const;
    double operator[](double t) const;
    void optimize();
public:
    Polynom(const std::vector<double>& koefs_ = {});
    Polynom(double koef_);

    int getPower() const;
    double getVal(double t) const;
    double operator()(double t) const;


    void reverse();

    Polynom operator-() const;

    friend Polynom operator +(const Polynom& a, const Polynom& b);
    friend Polynom operator *(const Polynom& a, const Polynom& b);

    friend Polynom operator -(const Polynom& a, const Polynom& b);
    friend Polynom operator /(const Polynom& a, const Polynom& b);
    friend Polynom operator %(const Polynom& a, const Polynom& b);

    friend Polynom& operator +=(Polynom& a, const Polynom& b);
    friend Polynom& operator *=(Polynom& a, const Polynom& b);
    friend Polynom& operator -=(Polynom& a, const Polynom& b);
    friend Polynom& operator /=(Polynom& a, const Polynom& b);
    friend Polynom& operator %=(Polynom& a, const Polynom& b);
};



#endif // POLYNOM_H

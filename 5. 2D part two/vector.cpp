#include "vector.h"
#include <cmath>
#include <stdexcept>
Vector::Vector():Vector(0,0){}
Vector::Vector(double x, double y):x(x),y(y){ }
Vector::Vector(QPointF a, QPointF b):x(b.x()-a.x()),y(b.y()-a.y()){}
Vector::Vector(const Vector& obj):Vector(obj.x,obj.y) { }
double Vector::getX() const{
    return x;
}
double Vector::getY() const{
    return y;
}
double Vector::getModule() const{
    return std::sqrt(x*x+y*y);
}
double Vector::operator()(){
    return getModule();
}
int Vector::getOctant() const{
    if (x > y && y >= 0){
        return 1;
    }
    else if (y >= x && x>0){
        return 2;
    }
    else if (x<=0 && y > -x){
        return 3;
    }
    else if (y>0 && y <= -x){
        return 4;
    }
    else if (y <= 0 && y > x){
        return 5;
    }
    else if (x < 0 && y <= x){
        return 6;
    }
    else if (x>=0 && y < -x){
        return 7;
    }
    else if (y<0 && y >= -x){
        return 8;
    }
    return -1;
}

void Vector::normalize(){
    (*this)/=getModule();
}
Vector Vector::normalized() const{
    Vector res(*this);
    res.normalize();
    return res;
}
Vector& Vector::operator=(const Vector& obj){
    if (&obj != this){
        x=obj.x;
        y=obj.y;
    }
    return *this;
}

Vector Vector::operator-() const{
    return (*this) * (-1);
}
Vector Vector::operator+(const Vector& obj) const{
    Vector res;
    res.x = x + obj.x;
    res.y = y + obj.y;
    return res;
}
Vector Vector::operator*(double val) const{
    Vector res;
    res.x=x*val;
    res.y=y*val;
    return res;
}
Vector operator*(double val, const Vector& obj){
    return obj*val;
}

double Vector::operator*(const Vector& obj) const{ // scalar mult
    return x*obj.x+y*obj.y;
}
double Vector::operator^(const Vector& obj) const{ // vector mult
    return x*obj.y-y*obj.x;
}
Vector Vector::operator/(double val) const{
    if (val == 0){
        throw std::invalid_argument("Zero Division in vectors");
    }
    return (*this) * (1/val);
}

Vector Vector::operator-(const Vector& obj) const{
    return (*this) + (-obj);
}
Vector& Vector::operator+=(const Vector& obj){
    (*this) = (*this)+obj;
    return *this;
}
Vector& Vector::operator-=(const Vector& obj){
    (*this) += (-obj);
    return *this;
}
Vector& Vector::operator/=(double val){
    (*this) = (*this)/val;
    return *this;
}
Vector& Vector::operator*=(double val){
    (*this) = (*this)*val;
    return *this;
}
bool Vector::operator==(const Vector& obj) const{
    return (x==obj.getX() && y==obj.getY());
}
bool Vector::operator!=(const Vector& obj) const{
    return !(*this == obj);
}
double scalarMult(const Vector& a, const Vector& b){
    return a*b;
}
double VectorMult(const Vector& a, const Vector& b){
    return a^b;
}

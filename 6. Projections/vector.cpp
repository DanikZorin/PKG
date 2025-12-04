#include "vector.h"
#include <cmath>
Vector::Vector():Vector(0,0,0){}
Vector::Vector(double x, double y, double z):x(x),y(y),z(z) { }
Vector::Vector(const matrix<3,1, double>& col):Vector(col[0][0],col[1][0],col[2][0]) { }
Vector::Vector(const Vector& obj):Vector(obj.x,obj.y,obj.z) { }
double Vector::getX() const{
    return x;
}
double Vector::getY() const{
    return y;
}
double Vector::getZ() const{
    return z;
}
double Vector::getModule() const{
    return std::sqrt(x*x+y*y+z*z);
}
double Vector::operator()(){
    return getModule();
}
void Vector::normalize(){
    (*this)/=getModule();
}
Vector Vector::normalized() const{
    Vector res(*this);
    res.normalize();
    return res;
}

Vector Vector::transformed(matrix<3,3, double> F) const{
    Vector res(*this);
    res.transform(F);
    return res;
}
void Vector::transform(matrix<3,3, double> F){
    matrix<3,1, double> mVec({{x},{y},{z}});
    mVec = F*mVec;
    x = mVec[0][0];
    y = mVec[1][0];
    z = mVec[2][0];
}

Vector& Vector::operator=(const Vector& obj){
    if (&obj != this){
        x=obj.x;
        y=obj.y;
        z=obj.z;
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
    res.z = z + obj.z;
    return res;
}
Vector Vector::operator*(double val) const{
    Vector res;
    res.x=x*val;
    res.y=y*val;
    res.z=z*val;
    return res;
}
Vector operator*(double val, const Vector& obj){
    return obj*val;
}

double Vector::operator*(const Vector& obj) const{ // scalar mult
    return x*obj.x+y*obj.y+z*obj.z;
}
Vector Vector::operator^(const Vector& obj) const{ // vector mult
    matrix<2,2> i({{    y,    z}
                  ,{obj.y,obj.z}});
    matrix<2,2> j({{    x,    z}
                  ,{obj.x,obj.z}});
    matrix<2,2> k({{    x,    y}
                  ,{obj.x,obj.y}});


    return Vector(
        i.det()
        ,-j.det()
        ,k.det()
        );
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
Vector& Vector::operator^=(const Vector& obj){
    (*this) = (*this)^obj;
    return *this;
}
bool Vector::operator==(const Vector& obj) const{
    return (x==obj.getX() && y==obj.getY() && z==obj.getZ());
}
bool Vector::operator!=(const Vector& obj) const{
    return !(*this == obj);
}
double scalarMult(const Vector& a, const Vector& b){
    return a*b;
}
Vector VectorMult(const Vector& a, const Vector& b){
    return a^b;
}

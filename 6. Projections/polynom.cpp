#include "polynom.h"
double Polynom::getKoef(double t) const{
    if (t > getPower()){
        return 0;
    }
    return koefs[t];
}
double Polynom::operator[](double t) const{
    return getKoef(t);
}
void Polynom::optimize(){
    for (int i=getPower();i>=0;i--){
        if (koefs[i] != 0){
            koefs.resize(i+1);
            break;
        }
    }
}
Polynom::Polynom(const std::vector<double>& koefs_): koefs(koefs_){
    optimize();
}
Polynom::Polynom(double koef_):Polynom(std::vector<double>({koef_})){}

int Polynom::getPower() const{
    return koefs.size()-1;
}
double Polynom::getVal(double t) const{
    double tCur = 1;
    double val = 0;
    for (int i=0;i<koefs.size();i++){
        val += tCur * koefs[i];
        tCur *= t;
    }
    return val;
}
double Polynom::operator()(double t) const{
    return getVal(t);
}


void Polynom::reverse(){
    for (int i=0;i<koefs.size()/2;i++){
        std::swap(koefs[i], koefs[koefs.size()-i-1]);
    }
}
Polynom Polynom::operator-() const{
    std::vector<double> koefs_;
    for (int i=0;i<koefs.size();i++){
        koefs_.push_back(-koefs[i]);
    }
    return koefs_;
}

Polynom operator +(const Polynom& a, const Polynom& b){
    int n = std::max(a.koefs.size(), b.koefs.size());
    std::vector<double> koefs_(n);
    for (int i=0;i<n;i++){
        koefs_[i] = a[i]+b[i];
    }
    return koefs_;
}

Polynom operator *(const Polynom& a, const Polynom& b){
    std::vector<double> koefs_(a.getPower()+b.getPower()+1, 0);
    for (int i=0;i<a.koefs.size();i++){
        for (int j=0;j<b.koefs.size();j++){
            koefs_[i+j]+=a[i]*b[j];
        }
    }
    return koefs_;
}

Polynom operator -(const Polynom& a, const Polynom& b){
    return a+(-b);
}
Polynom operator /(const Polynom& a, const Polynom& b){
    if (b.koefs.size() == 0){
        throw std::invalid_argument("zero division");
    }
    if (b.getPower() > a.getPower()){
        return 0;
    }
    std::vector<double> d(a.koefs);
    std::vector<double> res(a.getPower()-b.getPower()+1);
    for (int i=a.getPower(); i >= b.getPower();i--){
        double koef = d[i]/b[0];
        res[i-b.getPower()] = koef;
        for (int j=0;j<=b.getPower();j++){
            d[i-j] -= b[j]*koef;
        }
    }
    return res;
}
Polynom operator %(const Polynom& a, const Polynom& b){
    if (b.koefs.size() == 0){
        throw std::invalid_argument("zero division");
    }
    return a - (a/b)*b;
}


Polynom& operator +=(Polynom& a, const Polynom& b){
    a=a+b;
    return a;
}

Polynom& operator *=(Polynom& a, const Polynom& b){
    a=a*b;
    return a;
}

Polynom& operator -=(Polynom& a, const Polynom& b){
    a=a-b;
    return a;
}

Polynom& operator /=(Polynom& a, const Polynom& b){
    a=a/b;
    return a;
}
Polynom& operator %=(Polynom& a, const Polynom& b){
    a=a%b;
    return a;
}

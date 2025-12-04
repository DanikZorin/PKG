#include "shape.h"

void SurfaceInterpolated::copy(const SurfaceInterpolated& obj){
    Surface::copy(obj);
    us = obj.us;

    ls1 = obj.ls1;
    ls2 = obj.ls2;
}

SurfaceInterpolated::SurfaceInterpolated(): Surface(){

}
SurfaceInterpolated::SurfaceInterpolated(const QVector<QVector<Dot>>& us_, QColor color_): SurfaceInterpolated(){
    setBorderColor(color_);
    us = us_;
    vs = QVector<QVector<Vector>>(2, QVector<Vector>(us[0].size(), Vector(0,0,0)));
    ws = QVector<QVector<Vector>>(us.size(), QVector<Vector>(2, Vector(0,0,0)));

    updateKoefs();
}
SurfaceInterpolated::SurfaceInterpolated(const SurfaceInterpolated& obj){
    copy(obj);
}

SurfaceInterpolated::~SurfaceInterpolated(){

}

void SurfaceInterpolated::updateKoefs(){
    int n = us.size();
    int m = us[0].size();

    matrix_light<qreal> Q1(n+2,n+2);
    for (int j=0;j<n;j++){
        Q1[0][j] = 1;
    }
    for (int j=n;j<n+2;j++){
        Q1[0][j] = 0;
        Q1[1][j] = 1;
    }

    for (int i=1;i<n+2;i++){
        for (int j=0;j<n;j++){
            Q1[i][j] = Q1[i-1][j] * j;
        }
    }
    for (int i=2;i<n+2;i++){
        Q1[i][n] = Q1[i-1][n] / (i-1) * i * 0;
        Q1[i][n+1] = Q1[i-1][n+1] / (i-1) * i * (n-1);
    }
    matrix_light<qreal> H1(Q1.inverted());
    ls1.clear();
    for (int i=0;i<H1.getN();i++){
        ls1.push_back(Polynom(H1.getRow(i)));
    }




    matrix_light<qreal> Q2(m+2,m+2);
    for (int j=0;j<m;j++){
        Q2[0][j] = 1;
    }
    for (int j=m;j<m+2;j++){
        Q2[0][j] = 0;
        Q2[1][j] = 1;
    }
    for (int i=1;i<m+2;i++){
        for (int j=0;j<m;j++){
            Q2[i][j] = Q2[i-1][j] * j;
        }
    }
    for (int i=2;i<m+2;i++){
        Q2[i][m] = Q2[i-1][m] / (i-1) * i * 0;
        Q2[i][m+1] = Q2[i-1][m+1] / (i-1) * i * (m-1);
    }
    matrix_light<qreal> H2(Q2.inverted());
    ls2.clear();
    for (int i=0;i<H2.getN();i++){
        ls2.push_back(Polynom(H2.getRow(i)));
    }

    updateSegments();
}

Dot SurfaceInterpolated::getDot(qreal t1, qreal t2) const{
    int n = us.size();
    int m = us[0].size();

    matrix_light<Dot> U(n+2,m+2);

    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            U[i][j] = us[i][j];
        }
    }
    for (int j=0;j<m;j++){
        U[n][j] = Dot() + vs[0][j];
        U[n+1][j] = Dot() + vs[1][j];
    }
    for (int i=0;i<n;i++){
        U[i][m] = Dot() + ws[i][0];
        U[i][m+1] = Dot() + ws[i][1];
    }
    U[n][m] = Dot() + k0;
    U[n][m+1] = Dot() + k1;
    U[n+1][m] = Dot() + k2;
    U[n+1][m+1] = Dot() + k3;

    matrix_light<qreal>L1(n+2,1);
    for (int i=0;i<n+2;i++){
        L1[i][0] = ls1[i].getVal(t1);
    }
    U = operator*<qreal, Dot, Dot>(L1.transposed(), U);
    matrix_light<qreal>L2(m+2,1);
    for (int i=0;i<m+2;i++){
        L2[i][0] = ls2[i].getVal(t2);
    }
    U = operator*<Dot, qreal, Dot>(U, L2);

    if (U.getN() != 1 && U.getM() != 1){
        throw std::invalid_argument("bad Mat");
    }

    U[0][0].setName(QString::number(t1)+" "+QString::number(t2));
    return U[0][0];

}

void SurfaceInterpolated::setDot(int i, int j, const Dot& u){
    us[i][j] = u;
    updateSegments();
}
void SurfaceInterpolated::setVecDir1(int i, int j, const Vector& v){
    vs[i][j] = v;
    updateSegments();
}
void SurfaceInterpolated::setVecDir2(int i, int j, const Vector& w){
    ws[i][j] = w;
    updateSegments();
}
void SurfaceInterpolated::setVecRot(int i, int j, const Vector& k){
    if (i == 0 && j == 0){
        k0 = k;
    }
    if (i == 0 && j == 1){
        k1 = k;
    }
    if (i == 1 && j == 0){
        k2 = k;
    }
    if (i == 1 && j == 1){
        k3 = k;
    }
}

Dot SurfaceInterpolated::getDot(int i, int j) const{
    return us[i][j];
}
Vector SurfaceInterpolated::getVecDir1(int i, int j) const{
    return vs[i][j];
}
Vector SurfaceInterpolated::getVecDir2(int i, int j) const{
    return ws[i][j];
}
Vector SurfaceInterpolated::getVecRot(int i, int j) const{
    if (i == 0 && j == 0){
        return k0;
    }
    if (i == 0 && j == 1){
        return k1;
    }
    if (i == 1 && j == 0){
        return k2;
    }
    if (i == 1 && j == 1){
        return k3;
    }
}

void SurfaceInterpolated::setColor(const QColor& color){
    for (int i=0;i<us.size();i++){
        for (Dot& dot: us[i]){
            dot.setColor(color);
        }
    }
    updateSegments();
}
qreal SurfaceInterpolated::getTEnd1() const{
    return us.size()-1;
}
qreal SurfaceInterpolated::getTEnd2() const{
    return us[0].size()-1;
}

SurfaceInterpolated SurfaceInterpolated::transformed(const matrix<4,4>& F) const{
    SurfaceInterpolated res(*this);
    res.transform(F);
    return res;
}
void SurfaceInterpolated::transform(const matrix<4,4>& F){
    Surface::transform(F);
    for (int i=0;i<us.size();i++){
        for (Dot& dot: us[i]){
            dot.transform(F);
        }
    }
    updateSegments();
}

SurfaceInterpolated* SurfaceInterpolated::createCopy() const{
    return new SurfaceInterpolated(*this);
}

SurfaceInterpolated& SurfaceInterpolated::operator =(const SurfaceInterpolated& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

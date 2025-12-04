#include "shape.h"
SurfaceErmit::SurfaceErmit(): Surface(){

    matrix_light<qreal> T({
       {1,1,0,0},
       {0,1,1,1},
       {0,1,0,2},
       {0,1,0,3}
    });

    T.invert();

    for (int i=0;i<4;i++){
        ls.push_back(Polynom(T.getRow(i)));
    }
}
SurfaceErmit::SurfaceErmit(const Dot& u00_, const Dot& u01_, const Dot& u10_, const Dot& u11_, QColor borderColor_): SurfaceErmit(){
    setDot(0, u00_);
    setDot(1, u01_);
    setDot(2, u10_);
    setDot(3, u11_);
    setBorderColor(borderColor_);
}

SurfaceErmit::SurfaceErmit(const SurfaceErmit& obj): SurfaceErmit(){
    *this = obj;
}

SurfaceErmit::~SurfaceErmit(){ }

void SurfaceErmit::copy(const SurfaceErmit& obj){
    Surface::copy(obj);
    for (int i=0;i<4;i++){
        setNode(i, obj.getDot(i), obj.getVecDir1(i), obj.getVecDir2(i), obj.getVecRot(i));
    }
}

Dot SurfaceErmit::getDot(qreal t1, qreal t2) const{
    matrix_light<Dot> U({
        {u00, u01, Dot(0,0,0)+w00, Dot(0,0,0)+w01},
        {u10, u11, Dot(0,0,0)+w10, Dot(0,0,0)+w11},
        {Dot(0,0,0)+v00, Dot(0,0,0)+v01, Dot(0,0,0)+k00, Dot(0,0,0)+k01},
        {Dot(0,0,0)+v10, Dot(0,0,0)+v11, Dot(0,0,0)+k10, Dot(0,0,0)+k11},
    });
    matrix_light<qreal>L1({
        {ls[0].getVal(t1)},
        {ls[1].getVal(t1)},
        {ls[2].getVal(t1)},
        {ls[3].getVal(t1)},
    });
    U = operator*<qreal, Dot, Dot>(L1.transposed(), U);
    matrix_light<qreal>L2({
        {ls[0].getVal(t2)},
        {ls[1].getVal(t2)},
        {ls[2].getVal(t2)},
        {ls[3].getVal(t2)},
    });
    U = operator*<Dot, qreal, Dot>(U, L2);

    if (U.getN() != 1 && U.getM() != 1){
        throw std::invalid_argument("bad Mat");
    }
    //qDebug()<<u00.getColor().red()<<u01.getColor().red()<<u10.getColor().red()<<u11.getColor().red();
    //qDebug()<<U[0][0].getColor().red();
    U[0][0].setName(QString::number(t1)+" "+QString::number(t2));
    return U[0][0];
}
void SurfaceErmit::setDot(int i, const Dot& u){
    if (i == 0){
        u00 = u;
    }
    if (i == 1){
        u01 = u;
    }
    if (i == 2){
        u10 = u;
    }
    if (i == 3){
        u11 = u;
    }
    updateSegments();
}
void SurfaceErmit::setVecDir1(int i, const Vector& v){
    if (i == 0){
        v00 = v;
    }
    if (i == 1){
        v01 = v;
    }
    if (i == 2){
        v10 = v;
    }
    if (i == 3){
        v11 = v;
    }
    updateSegments();
}
void SurfaceErmit::setVecDir2(int i, const Vector& w){
    if (i == 0){
        w00 = w;
    }
    if (i == 1){
        w01 = w;
    }
    if (i == 2){
        w10 = w;
    }
    if (i == 3){
        w11 = w;
    }
    updateSegments();
}
void SurfaceErmit::setVecRot(int i, const Vector& k){
    if (i == 0){
        k00 = k;
    }
    if (i == 1){
        k01 = k;
    }
    if (i == 2){
        k10 = k;
    }
    if (i == 3){
        k11 = k;
    }
    updateSegments();
}
void SurfaceErmit::setNode(int i, const Dot& u, const Vector& v, const Vector& w, const Vector& k){
    if (i == 0){
        u00 = u;
        v00 = v;
        w00 = w;
        k00 = k;
    }
    if (i == 1){
        u01 = u;
        v01 = v;
        w01 = w;
        k01 = k;
    }
    if (i == 2){
        u10 = u;
        v10 = v;
        w10 = w;
        k10 = k;
    }
    if (i == 3){
        u11 = u;
        v11 = v;
        w11 = w;
        k11 = k;
    }
    updateSegments();
}

Dot SurfaceErmit::getDot(int i) const{
    if (i == 0){
        return u00;
    }
    if (i == 1){
        return u01;
    }
    if (i == 2){
        return u10;
    }
    if (i == 3){
        return u11;
    }
    throw std::invalid_argument("getDot bad arg");
}
Vector SurfaceErmit::getVecDir1(int i) const{
    if (i == 0){
        return v00;
    }
    if (i == 1){
        return v01;
    }
    if (i == 2){
        return v10;
    }
    if (i == 3){
        return v11;
    }
}
Vector SurfaceErmit::getVecDir2(int i) const{
    if (i == 0){
        return w00;
    }
    if (i == 1){
        return w01;
    }
    if (i == 2){
        return w10;
    }
    if (i == 3){
        return w11;
    }
}
Vector SurfaceErmit::getVecRot(int i) const{
    if (i == 0){
        return k00;
    }
    if (i == 1){
        return k01;
    }
    if (i == 2){
        return k10;
    }
    if (i == 3){
        return k11;
    }
}
void SurfaceErmit::setColor(const QColor& color){
    u00.setColor(color);
    u01.setColor(color);
    u10.setColor(color);
    u11.setColor(color);
    updateSegments();
}

SurfaceErmit SurfaceErmit::transformed(const matrix<4,4>& F) const{
    SurfaceErmit res(*this);
    res.transform(F);
    return res;
}
void SurfaceErmit::transform(const matrix<4,4>& F){
    Surface::transform(F);
    u00.transform(F);
    u01.transform(F);
    u10.transform(F);
    u11.transform(F);
    v00.transform(matrix<3,3>(F));
    v01.transform(matrix<3,3>(F));
    v10.transform(matrix<3,3>(F));
    v11.transform(matrix<3,3>(F));
    w00.transform(matrix<3,3>(F));
    w01.transform(matrix<3,3>(F));
    w10.transform(matrix<3,3>(F));
    w11.transform(matrix<3,3>(F));
    k00.transform(matrix<3,3>(F));
    k01.transform(matrix<3,3>(F));
    k10.transform(matrix<3,3>(F));
    k11.transform(matrix<3,3>(F));
}

SurfaceErmit* SurfaceErmit::createCopy() const{
    return new SurfaceErmit(*this);
}

SurfaceErmit& SurfaceErmit::operator =(const SurfaceErmit& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

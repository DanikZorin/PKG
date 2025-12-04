#include "shape.h"

SurfaceKinematic::SurfaceKinematic(): Surface(){
    F = 0;
    main = 0;
    dir = 0;
}
SurfaceKinematic::SurfaceKinematic(const Spline& main_, const Spline& dir_, Dot (*F_)(Dot, qreal, qreal), QColor color)
    : SurfaceKinematic(){
    main = main_.createCopy();
    dir = dir_.createCopy();
    F = F_;
    setBorderColor(color);
    updateSegments();
}

SurfaceKinematic::SurfaceKinematic(const SurfaceKinematic& obj): SurfaceKinematic(){
    copy(obj);
}

SurfaceKinematic::~SurfaceKinematic(){
    delete main;
    delete dir;
}

void SurfaceKinematic::copy(const SurfaceKinematic& obj){
    Surface::copy(obj);
    if (main != 0){
        delete main;
    }
    if (dir != 0){
        delete dir;
    }
    main = obj.main->createCopy();
    dir = obj.dir->createCopy();
    setF(obj.F);
}
void SurfaceKinematic::setMainSpline(const Spline& spline){
    delete main;
    main = spline.createCopy();
    updateSegments();
}
void SurfaceKinematic::setDirSpline(const Spline& spline){
    delete dir;
    dir = spline.createCopy();
    updateSegments();
}
void SurfaceKinematic::setF(Dot (*F_)(Dot, qreal, qreal)){
    F = F_;
    updateSegments();
}

void SurfaceKinematic::setSubNodesMain(int cnt_){
    setSubNodes1(cnt_);
}
void SurfaceKinematic::setSubNodesDir(int cnt_){
    setSubNodes2(cnt_);
}

Dot SurfaceKinematic::getDot(qreal t1, qreal t2) const{
    Dot res = main->getDot(t1*main->getTEnd());
    res.setCenter(main->getCenter());
    if (F != 0){
        res = F(res, t1, t2);
    }
    res = res + dir->getDot(t2*dir->getTEnd());
    res.transform(Fpoints);
    res.setName(QString::number(t1)+" "+QString::number(t2));

    return res;
}
void SurfaceKinematic::setColor(const QColor& color){
    main->setColor(color);
    dir->setColor(color);
    updateSegments();
}

SurfaceKinematic SurfaceKinematic::transformed(const matrix<4,4>& F) const{
    SurfaceKinematic res(*this);
    res.transform(F);
    return res;
}
void SurfaceKinematic::transform(const matrix<4,4>& F){
    Surface::transform(F);
    Fpoints = F * Fpoints;
    updateSegments();
}
SurfaceKinematic* SurfaceKinematic::createCopy() const{
    return new SurfaceKinematic(*this);
}

SurfaceKinematic& SurfaceKinematic::operator =(const SurfaceKinematic& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;

}

#include "shape.h"
void SplineKinematic::copy(const SplineKinematic& obj){
    Spline::copy(obj);
    Fpoints = obj.Fpoints;
    F = obj.F;

    tStart = obj.tStart;
    tEnd = obj.tEnd;
    begin = obj.begin;

    updateSegments();
}
SplineKinematic::SplineKinematic(Dot begin_, Dot (*F_)(Dot dot, qreal t), qreal tEnd_): Spline(){
    begin = begin_;
    setCenter(Dot(0,0,0));
    begin.setCenter(Dot(0,0,0));
    F = F_;
    tStart = 0;
    tEnd = tEnd_;
    updateSegments();
}
SplineKinematic::SplineKinematic(const SplineKinematic& obj){
    copy(obj);
}

void SplineKinematic::setTEnd(qreal t_){
    tEnd = t_;
}
void SplineKinematic::setTStart(qreal t_){
    tStart = t_;
}

Dot SplineKinematic::getDot(qreal t) const{
    if (F == 0){
        return begin.transformed(Fpoints);
    }
    return F(begin, t).transformed(Fpoints);
}
qreal SplineKinematic::getTEnd() const{
    return tEnd;
}
void SplineKinematic::setColor(const QColor& color){
    begin.setColor(color);
}

SplineKinematic SplineKinematic::transformed(const matrix<4,4>& F) const{
    SplineKinematic res(*this);
    res.transform(F);
    return res;
}
void SplineKinematic::transform(const matrix<4,4>& F){
    Spline::transform(F);
    Fpoints = F * Fpoints;
    updateSegments();
}

SplineKinematic* SplineKinematic::createCopy() const{
    return new SplineKinematic(*this);
}

SplineKinematic& SplineKinematic::operator=(const SplineKinematic& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

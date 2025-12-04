#include "shape.h"
void Spline::copy(const Spline& obj){
    Shape::copy(obj);
    subNodes = obj.subNodes;
}
Spline::Spline():Shape(), subNodes(-1){ }
Spline::~Spline(){}

void Spline::updateSegments(){
    segments.clearShapes();

    QVector<Dot> dots;

    for (int i=0;i<subNodes;i++){
        qreal t = (qreal)(i)/(subNodes-1)*getTEnd();
        dots.push_back(getDot(t));
    }

    for (int i=0;i<dots.size()-1;i++){
        Line line(dots[i],dots[i+1]);
        segments.addShape(&line);
    }
    for (Shape* shape: segments.getShapes()){
        shape->setCenter(getCenter());
    }
}
qreal Spline::getTEnd() const{
    return 1;
}

void Spline::setSubNodes(int cnt_){
    subNodes = cnt_;
    updateSegments();
}
Dot Spline::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }
    return segments.getCenter();
}
void Spline::transform(const matrix<4,4,qreal>& F){
    Shape::transform(F);
    segments.transform(F);
}
void Spline::draw(const DrawReq& drawReq) const{
    segments.draw(drawReq);
}

#include "shape.h"
Surface::Surface():Shape(){
    subNodes1 = 2;
    subNodes2 = 2;
    isBorder = true;
}
void Surface::updateSegments(){
    segments.clearShapes();
    QVector<QVector<Dot>> dots(subNodes1);
    for (int i=0;i<subNodes1;i++){
        qreal t1 = (qreal)(i)/(subNodes1-1)*getTEnd1();
        for (int j=0;j<subNodes2;j++){
            qreal t2 = (qreal)(j)/(subNodes2-1)*getTEnd2();

            dots[i].push_back(getDot(t1,t2));
        }
    }
    for (int i=0;i<dots.size()-1;i++){
        for (int j=0;j<dots[i].size()-1;j++){
            Polygon polygon({dots[i][j],dots[i][j+1],dots[i+1][j+1],dots[i+1][j]}, borderColor);
 //           qDebug()<<polygon.getDots()[0].getCenterSet();
         //   polygon.getDots()[0].show();
            polygon.setBorderState(isBorder);
            polygon.setBackColorState(isPolygonBack);
            segments.addShape(&polygon);
        }
    }

}

void Surface::setSubNodes(int cnt_){
    setSubNodes1(cnt_);
    setSubNodes2(cnt_);
}
void Surface::setSubNodes1(int cnt_){
    subNodes1 = cnt_;
    updateSegments();
}
void Surface::setSubNodes2(int cnt_){
    subNodes2 = cnt_;
    updateSegments();
}
int Surface::getSubNodes1() const{
    return subNodes1;
}
int Surface::getSubNodes2() const{
    return subNodes2;
}
void Surface::setBorderColor(const QColor& color_){
    borderColor = color_;
    if (!isBorder){
        borderOn();
    }
}
void Surface::borderOff(){
    isBorder = false;
    updateSegments();
}
void Surface::borderOn(){
    isBorder = true;
    updateSegments();
}
void Surface::polygonBackOff(){
    isPolygonBack = false;
    updateSegments();
}
void Surface::polygonBackOn(){
    isPolygonBack = true;
    updateSegments();
}

qreal Surface::getTEnd1() const{
    return 1;
}
qreal Surface::getTEnd2() const{
    return 1;
}
void Surface::transform(const matrix<4,4,qreal>& F){
    Shape::transform(F);
    segments.transform(F);
}
void Surface::copy(const Surface& obj){
    Shape::copy(obj);
    subNodes1 = obj.subNodes1;
    subNodes2 = obj.subNodes2;
    segments = obj.segments;
}
QVector<Polygon> Surface::extractPolygons() const{
    return segments.extractPolygons();
}

Dot Surface::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }
    return segments.getCenter();
}
void Surface::draw(const DrawReq& drawReq) const{
    segments.draw(drawReq);
}

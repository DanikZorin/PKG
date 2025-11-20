#include "shape.h"
void Line::copy(const Line* obj){
    Shape::copy(obj);
    this->x0 = obj->x0;
    this->y0 = obj->y0;
    this->x1 = obj->x1;
    this->y1 = obj->y1;
}
Shape::Shape(QColor clr) {
    setColor(clr);
}
Shape::Shape(const Shape* obj){
    copy(obj);
}
Shape::~Shape(){

}
QColor Shape::getColor() const{    
    return clr;
}
void Shape::setColor(const QColor& clr){
    this->clr = clr;
}
QPointF Shape::transform(DrawReq& drawReq, QPointF point){
    QSize screenCntr = drawReq.context->size();
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();

    return {screenCntr.width()/2 +(point.x()+drawReq.xCntr)*scaleKoef + drawReq.xAskew -1,
            screenCntr.height()/2+(-point.y()+drawReq.yCntr)*scaleKoef + drawReq.yAskew-1};
}

void Shape::copy(const Shape* obj){
    setColor(obj->getColor());
}

Line::Line(QColor clr): Shape(clr){

}
Line::Line(qreal x0, qreal y0, qreal x1, qreal y1, QColor clr): Line(clr){
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
}
Line::Line(QPointF p0, QPointF p1, QColor clr):Line(p0.x(),p0.y(),p1.x(),p1.y(),clr){

}

Line::Line(const Line* obj): Line(){
    copy(obj);
}
QPointF Line::getStart() const{
    return {x0,y0};
}
QPointF Line::getEnd() const{
    return {x1,y1};
}
void Line::paint(DrawReq& drawReq){
    QPainter p(drawReq.context);

    p.setBrush(clr);
    p.setPen(QPen(clr, 20*drawReq.zoom));


    p.drawLine(transform(drawReq, {x0,y0}), transform(drawReq, {x1,y1}));
}

Line* Line::createCopy(){
    return new Line(this);
}

void Polygon::copy(const Polygon* obj){
    Shape::copy(obj);
    this->points = obj->points;
}
void Polygon::checkRegularity(){
    if (points.size() != 4){
        isReagularWindow = false;
        return;
    }

    Vector v1(points[0], points[1]);
    Vector v2(points[1], points[2]);
    Vector v3(points[2], points[3]);
    Vector v4(points[3], points[0]);

    if (v1 != -v3 || v2 != -v4){
        isReagularWindow = false;
        return;
    }
    if (!(v1.getModule() == qAbs(v1.getX())||v1.getModule() == qAbs(v1.getY()))){
        isReagularWindow = false;
        return;
    }
    if (!(v2.getModule() == qAbs(v2.getX())||v2.getModule() == qAbs(v2.getY()))){
        isReagularWindow = false;
        return;
    }

    xMx = qMax(qMax(points[0].x(),points[1].x()),qMax(points[2].x(),points[3].x()));
    yMx = qMax(qMax(points[0].y(),points[1].y()),qMax(points[2].y(),points[3].y()));
    xMn = qMin(qMin(points[0].x(),points[1].x()),qMin(points[2].x(),points[3].x()));
    yMn = qMin(qMin(points[0].y(),points[1].y()),qMin(points[2].y(),points[3].y()));

    isReagularWindow = true;
}
bool Polygon::isLeft() const{
    int octSum = 0;

    QVector <QPointF> p = points;
    p.push_back(points[0]);

    int oct[points.size()+1];
    for (int i=0;i<points.size();i++){
        Vector v(p[i],p[i+1]);
        oct[i] = v.getOctant();
    }
    oct[points.size()] = oct[0];

    int dOct[points.size()];

    for (int i=0;i<points.size();i++){
        dOct[i] = oct[i+1]-oct[i];
        if (dOct[i] > 4) dOct[i] -=8;
        if (dOct[i] < -4) dOct[i] +=8;
        if (qAbs(dOct[i]) == 4){
            Vector v0(p[(i-1+points.size())%points.size()],p[i]);
            Vector v1(p[i], p[i+1]);

            if (VectorMult(v0,v1) > 0) dOct[i] = 4;
            if (VectorMult(v0,v1) == 0) dOct[i] = 0;
            if (VectorMult(v0,v1) < 0) dOct[i] = -4;
        }

    }

    for (int i=0;i<points.size();i++){
        octSum += dOct[i];
    }

    qDebug()<<octSum;
    return octSum > 0;
}
void Polygon::reverse(){
    for (int i=0;i<points.size()/2;i++){
        qSwap(points[i],points[points.size()-1-i]);
    }
}
Polygon::Polygon(QColor clr):Shape(clr){

}
Polygon::Polygon(const QVector<QPointF>& points, QColor clr):Polygon(clr){
    this->points = points;
    checkRegularity();
    if (!isLeft())
        reverse();
}
Polygon::Polygon(const Polygon* obj){
    copy(obj);
}

void Polygon::addPoint(QPointF point){
    points.push_back(point);
}

void Polygon::paint(DrawReq& drawReq){
    QPainter p(drawReq.context);

    p.setBrush(clr);
    p.setPen(QPen(clr, 20*drawReq.zoom));


    QVector<QPointF> pointsT;

    for (QPointF point: points){
        pointsT.push_back(transform(drawReq, point));
    }

    p.drawPolygon(pointsT);
}
Polygon* Polygon::createCopy(){
    return new Polygon(this);
}

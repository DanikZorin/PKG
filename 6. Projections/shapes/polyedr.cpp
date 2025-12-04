#include "shape.h"

Polyedr::Polyedr(): Shape() { }
Polyedr::Polyedr(const QVector<Polygon>& polygons_): Polyedr(){
    for (const Polygon& polygon: polygons_){
        addPolygon(polygon);
    }
}
Polyedr::Polyedr(const Polyedr& obj): Polyedr(){
    (*this) = obj;
}
Polyedr::~Polyedr(){ }
void Polyedr::copy(const Polyedr& obj){
    Shape::copy(obj);
    polygons = obj.getPolygons();
    center = obj.getCenter();
}
void Polyedr::update(){
    qreal x=0,y=0,z=0;
    int cnt = 0;
    for (const Polygon& polygon: polygons){
        Dot center_ = polygon.center;
        x+=center_.getX();
        y+=center_.getY();
        z+=center_.getZ();
        cnt++;
    }
    center.setPoint(x,y,z);
    center/=cnt;
}
void Polyedr::addPolygon(const Polygon& polygon){
    polygons.push_back(polygon);
    update();
}
QVector<Polygon> Polyedr::getPolygons() const{
    return polygons;
}
Vector Polyedr::getNormalOut(const Polygon& polygon) const{
    Vector n = polygon.getNormal();
    Vector toCenter = center - polygon.getCenter();

    qreal val = n*toCenter;
    if (val > 0){
        n = -n;
    }
    return n;
}

bool Polyedr::isFacePolygon(const Polygon& polygon, const Vector& onViewer) const{
    if (getNormalOut(polygon)*onViewer > 0){
        return true;
    }
    else {
        return false;
    }
}
Polyedr Polyedr::expandLine(const Line& line, qreal extraWidth, Plato plato, QColor color, bool expandMain){
    Vector mainVec = line.getEnd() - line.getBegin();
    Vector xVec(extraWidth, 0, 0);
    Vector yVec(0, extraWidth, 0);
    Vector zVec(0, 0, extraWidth);

    qreal module = mainVec.getModule();
    Vector loose = mainVec / module * extraWidth / 2;
    if (expandMain)
        mainVec = mainVec / module * (module + extraWidth);
    Vector a, b, c;
    if (plato == Oxy){
        a = xVec;
        b = yVec;
    }
    if (plato == Oyz){
        a = yVec;
        b = zVec;
    }
    if (plato == Ozx){
        a = zVec;
        b = xVec;
    }
    return createPrism(line.getBegin() - (a + b) / 2 - loose*expandMain, mainVec, a, b, color);
}

Polyedr Polyedr::createPrism(const Dot& corner, Vector a, Vector b, Vector c, QColor borderColor){
    Polyedr prism;

    Dot d0 = corner;
    Dot d1 = corner + a;
    Dot d2 = corner + b;
    Dot d3 = corner + a + b;
    Dot d4 = corner + c;
    Dot d5 = corner + a + c;
    Dot d6 = corner + b + c;
    Dot d7 = corner + a + b + c;

    prism.addPolygon(Polygon({d0,d1,d5,d4}, borderColor));
    prism.addPolygon(Polygon({d0,d2,d3,d1}, borderColor));
    prism.addPolygon(Polygon({d0,d4,d6,d2}, borderColor));
    prism.addPolygon(Polygon({d7,d3,d2,d6}, borderColor));
    prism.addPolygon(Polygon({d7,d5,d1,d3}, borderColor));
    prism.addPolygon(Polygon({d7,d6,d4,d5}, borderColor));

    return prism;
}
void Polyedr::setColor(const QColor& color_){
    for (Polygon& polygon: polygons){
        polygon.setColor(color_);
    }
}
QVector<Polygon> Polyedr::extractPolygons() const{
    return polygons;
}
Dot Polyedr::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }

    return center;
}
Polyedr Polyedr::transformed(const matrix<4,4>& F) const{
    Polyedr res(*this);
    res.transform(F);
    return res;
}
void Polyedr::transform(const matrix<4,4>& F){
    Shape::transform(F);
    for (Polygon& polygon: polygons){
        polygon.transform(F);
    }
    update();
}

void Polyedr::draw(const DrawReq& drawReq) const{

    for (const Polygon& polygon: polygons){
        if (polygon.getDots().size()<3){
            continue;
        }
        if (isFacePolygon(polygon,(drawReq.projection == Central || drawReq.projection == Perspective?drawReq.viewer-polygon.getCenter():drawReq.onViewer) )){
            polygon.draw(drawReq);
        }
    }
}
Polyedr* Polyedr::createCopy() const{
    return new Polyedr(*this);
}

Polyedr& Polyedr::operator=(const Polyedr& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

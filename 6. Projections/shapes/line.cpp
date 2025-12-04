#include "shape.h"
void Line::copy(const Line& obj){
    Shape::copy(obj);
    begin = obj.begin;
    end = obj.end;
}
Line::Line(): Shape() {}
Line::Line(const Dot& begin_, const Dot& end_): Line(){
    begin = begin_;
    end = end_;
}
Line::Line(const Dot& begin_, const Dot& end_, QColor color_): Line(begin_, end_){
    setColor(color_);
}

Line::Line(const Line& obj): Line(){
    (*this) = obj;
}
Line::~Line(){ }

void Line::setBegin(qreal x, qreal y, qreal z){
    begin.setPoint(x,y,z);
}
void Line::setBegin(const Dot& begin_){
    begin = begin_;
}
void Line::setEnd(qreal x, qreal y, qreal z){
    end.setPoint(x,y,z);
}
void Line::setEnd(const Dot& end_){
    end = end_;
}
Dot Line::getBegin() const{
    return begin;
}
Dot Line::getEnd() const{
    return end;
}
void Line::setColor(const QColor& color){
    begin.setColor(color);
    end.setColor(color);
}
Dot Line::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }

    return (begin + end)/2;
}

Line Line::transformed(const matrix<4,4>& F) const{
    Line res(*this);
    res.transform(F);
    return res;
}
void Line::transform(const matrix<4,4>& F){
    Shape::transform(F);
    begin.transform(F);
    end.transform(F);
}
QPair<QPointF,QPointF> Line::getLineScreened(const Dot& begin, const Dot& end, const DrawReq& drawReq){

    matrix<4,2> mDots;

    mDots.setColumn(0, {begin.getX(),begin.getY(),begin.getZ(),1});
    mDots.setColumn(1, {end.getX(),end.getY(),end.getZ(),1});
    mDots = drawReq.F * mDots;

    matrix<3,2> mDotsP;

    QPointF pos1 = normalizeByPlato(mDots[0][0],mDots[1][0],mDots[2][0], drawReq.plato)
        ,pos2 = normalizeByPlato(mDots[0][1],mDots[1][1],mDots[2][1], drawReq.plato);

    mDotsP.setColumn(0, {pos1.x(), pos1.y(), mDots[3][0]});
    mDotsP.setColumn(1, {pos2.x(), pos2.y(), mDots[3][1]});

    for (int i=0;i<2;i++){
        if (mDotsP[2][0] != 0){
            mDotsP[i][0] /= mDotsP[2][0];
        }
        if (mDotsP[2][1] != 0){
            mDotsP[i][1] /= mDotsP[2][1];
        }
    }

    if (mDotsP[2][0] > 0 && mDotsP[2][1]==0){
        QPointF corner = Dot::extended(mDotsP[0][0],mDotsP[1][0], mDotsP[0][1], mDotsP[1][1], drawReq);
        return {{mDotsP[0][0], mDotsP[1][0]}, {corner.x(), corner.y()}};
    }
    if (mDotsP[2][0]==0 && mDotsP[2][1] > 0){
        QPointF corner = Dot::extended(mDotsP[0][1],mDotsP[1][1], mDotsP[0][0], mDotsP[1][0], drawReq);
        return {{mDotsP[0][1], mDotsP[1][1]}, {corner.x(), corner.y()}};
    }
    if (mDotsP[2][0] > 0 && mDotsP[2][1] > 0){
        return {{mDotsP[0][0], mDotsP[1][0]}, {mDotsP[0][1], mDotsP[1][1]}};
    }
    if (mDotsP[2][0] < 0 && mDotsP[2][1] > 0){
        QPointF corner = Dot::reverse(mDotsP[0][1], mDotsP[1][1], mDotsP[0][0], mDotsP[1][0], drawReq);
        return { {corner.x(), corner.y()}, {mDotsP[0][1], mDotsP[1][1]} };
    }
    if (mDotsP[2][0] > 0 && mDotsP[2][1] < 0){
        QPointF corner = Dot::reverse(mDotsP[0][0], mDotsP[1][0], mDotsP[0][1], mDotsP[1][1], drawReq);
        return {{mDotsP[0][0], mDotsP[1][0]}, {corner.x(), corner.y()}};
    }

    throw std::invalid_argument("bad line screen / no result");
}
void Line::draw(qreal x1,qreal y1, qreal x2, qreal y2, const DrawReq& drawReq) const{
    qreal koef = getKoef(drawReq.context);
    QPainter p(drawReq.context);


    QPointF begin_(drawReq.context->width()/2 + x1*koef/2*drawReq.zoom, drawReq.context->height()/2 - y1*koef/2*drawReq.zoom);
    QPointF end_(drawReq.context->width()/2 + x2*koef/2*drawReq.zoom, drawReq.context->height()/2 - y2*koef/2*drawReq.zoom);

    QLinearGradient color(begin_, end_);
    color.setColorAt(0., begin.getColor());
    color.setColorAt(1., end.getColor());

    p.setPen(QPen(color, 1));

    p.drawLine(begin_, end_);

    p.end();
}
void Line::draw(const DrawReq& drawReq) const{
    try{
        QPair<QPointF, QPointF> line = Line::getLineScreened(begin, end, drawReq);
        draw(line.first.x(),line.first.y(),line.second.x(),line.second.y(), drawReq);
    }
    catch(std::invalid_argument){

    }
}
Line* Line::createCopy() const{
    return new Line(*this);
}

Line& Line::operator =(const Line& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

#include "shape.h"

QColor createColor(const matrix<3,1, qreal>& colorM){
    qreal r = std::min(1., std::max(0., colorM[0][0]));
    qreal g = std::min(1., std::max(0., colorM[1][0]));
    qreal b = std::min(1., std::max(0., colorM[2][0]));

    return QColor(r*255,g*255,b*255);
}

void Dot::copy(const Dot& obj){
    Shape::copy(obj);
    x = obj.getX();
    y = obj.getY();
    z = obj.getZ();
    colorM = obj.colorM;
}
Dot::Dot(): Shape() {
    setPoint(0, 0, 0);
    setColor(Qt::black);
}
Dot::Dot(qreal x_, qreal y_, qreal z_, QColor color_): Dot(){
    setPoint(x_, y_, z_);
    setColor(color_);
}
Dot::Dot(const Dot& obj): Dot(){
    copy(obj);
}

void Dot::setPoint(qreal x_, qreal y_, qreal z_){
    x=x_;
    y=y_;
    z=z_;
}
qreal Dot::getX() const{
    return x;
}
qreal Dot::getY() const{
    return y;
}
qreal Dot::getZ() const{
    return z;
}
void Dot::show() const{
    qDebug()<<name<<x<<y<<z;
}
void Dot::setColor(const QColor& color_){
    colorM[0][0] = color_.redF();
    colorM[1][0] = color_.greenF();
    colorM[2][0] = color_.blueF();
}
QColor Dot::getColor() const{
    return createColor(colorM);
}

QPointF Dot::extended(qreal x, qreal y, qreal dx, qreal dy, const DrawReq& drawReq){
    QWidget* parent = drawReq.context;
    qreal zoom = drawReq.zoom;

    qreal koef = getKoef(drawReq.context);

    qreal tEnd = 0;
    qreal x_ = parent->width()/2 + x*koef/2*zoom;
    qreal y_ = parent->height()/2 - y*koef/2*zoom;


    matrix<2,1, qreal> dirVec_({{(dx*koef/2*zoom)}
                          ,{(-dy*koef/2*zoom)}});


    qreal t;
    // 0 = x_ + dirVec_[0][0]*t;
    t = -x_ / dirVec_[0][0];
    if ((t>0)&&(tEnd == 0 || tEnd < t)){
        tEnd = t;
    }
    // parent->width() = x_ + dirVec_[0][0]*t;
    t = (parent->width()-x_) / dirVec_[0][0];
    if ((t>0)&&(tEnd == 0 || tEnd < t)){
        tEnd = t;
    }
    // 0 = y_ + dirVec_[1][0]*t;
    t = -y_ / dirVec_[1][0];
    if ((t>0)&&(tEnd == 0 || tEnd < t)){
        tEnd = t;
    }
    // parent->height() = y_ + dirVec_[1][0]*t;
    t = (parent->height()-y_) / (dirVec_[1][0]);
    if ((t>0)&&(tEnd == 0 || tEnd < t)){
        tEnd = t;
    }

    return QPointF(x+tEnd*dx, y+tEnd*dy);
}

QPointF Dot::reverse(qreal xReal, qreal yReal, qreal xFake, qreal yFake, const DrawReq& drawReq){
    return Dot::extended(xReal, yReal, xReal-xFake, yReal-yFake, drawReq);
}

Dot Dot::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }
    return *this;
}

Dot Dot::transformed(const matrix<4,4>& F) const{
    Dot res(*this);
    res.transform(F);
    return res;
}
void Dot::transform(const matrix<4,4>& F){
    Shape::transform(F);
    matrix<4,1> mDot;
    mDot.setColumn(0, {x,y,z,1});
    mDot = F * mDot;

    if (mDot[3][0] <= 0){
        qDebug()<<"Bad not affin transformation!";
        throw 1;
    }
    x = mDot[0][0]/mDot[3][0];
    y = mDot[1][0]/mDot[3][0];
    z = mDot[2][0]/mDot[3][0];

}

void Dot::draw(qreal x_, qreal y_, const DrawReq& drawReq) const{
    qreal koef = getKoef(drawReq.context);
    QPainter* p = new QPainter(drawReq.context);
    p->setPen(getColor());

    p->drawPoint(drawReq.context->width()/2+x_*koef/2*drawReq.zoom,
                 drawReq.context->height()/2-y_*koef/2*drawReq.zoom);

    p->end();
    delete p;
}

void Dot::draw(const DrawReq& drawReq) const{
    matrix<4,1> mDot;
    mDot.setColumn(0, {x,y,z,1});
    mDot = drawReq.F * mDot;

    if (mDot[3][0] > 0){
        QPointF pos = normalizeByPlato(mDot[0][0]/mDot[3][0],mDot[1][0]/mDot[3][0],mDot[2][0]/mDot[3][0], drawReq.plato);
        draw(pos.x(), pos.y(), drawReq);
    }
}
Dot* Dot::createCopy() const{
    return new Dot(*this);
}
Vector Dot::operator -(const Dot& obj) const{
    return Vector(x-obj.x, y-obj.y, z-obj.z);
}
Dot Dot::operator +(const Vector& vec) const{
    Dot res(*this);
    res.x += vec.getX();
    res.y += vec.getY();
    res.z += vec.getZ();
    return res;
}
Dot Dot::operator *(qreal val) const{
    Dot res;
    res.x = x * val;
    res.y = y * val;
    res.z = z * val;
    res.colorM = colorM * val;
    return res;
}
Dot operator*(qreal val, const Dot obj){
    return obj * val;
}

Dot Dot::operator +(const Dot& obj) const{
    Dot res;
    res.x = obj.getX() + x;
    res.y = obj.getY() + y;
    res.z = obj.getZ() + z;
    res.colorM = colorM + obj.colorM;
    return res;
}
Dot& Dot::operator +=(const Vector& vec){
    (*this) = *this + vec;
    return *this;
}
Dot& Dot::operator +=(const Dot& obj){
    *this = *this + obj;
    return *this;
}
Dot Dot::operator -(const Vector& vec) const{
    return (*this) + (-vec);
}
Dot& Dot::operator -=(const Vector& vec){
    (*this) += (-vec);
    return *this;
}
Dot& Dot::operator *=(qreal val){
    (*this) = (*this) * val;
    return *this;
}
Dot Dot::operator /(qreal vec) const{
    return (*this) * (1/vec);
}
Dot& Dot::operator /=(qreal val){
    (*this) = (*this) / val;
    return *this;
}
Dot operator *(matrix<4,4, qreal> F, const Dot& obj){
    Dot res(obj);
    matrix<4,1> objM({
        {obj.x},
        {obj.y},
        {obj.z},
        {1},
    });
    objM = F*objM;
    objM/=objM[3][0];
    res.setPoint(objM[0][0],objM[0][1],objM[0][2]);
    return res;
}

Dot& Dot::operator=(const Dot& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}

bool Dot::operator ==(const Dot& obj) const{
    return (x==obj.getX() && y==obj.getY() && z==obj.getZ());
}

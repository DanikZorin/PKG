#include "shape.h"

Polygon::Polygon(): Shape(){
    backColor = QColor(100, 100, 255);
    isBackColor = true;
    borderColor = Qt::black;
    isBorder = true;
}
Polygon::Polygon(QColor color_): Polygon(){
    setBorderColor(color_);
}
Polygon::Polygon(const QVector<Dot>& dots_, QColor color_):Polygon(color_){
    for (const Dot& dot: dots_){
        addDot(dot);
    }
}
Polygon::Polygon(const Polygon& obj): Polygon(){
    copy(obj);
}
Polygon::~Polygon() { }
void Polygon::copy(const Polygon& obj){
    Shape::copy(obj);
    dots = obj.getDots();
    center = obj.center;
    setBorderColor(obj.borderColor);    
    isBorder = obj.isBorder;
    isBackColor = obj.isBackColor;
}
void Polygon::update(){
    center = Dot(0,0,0,Qt::black);
    for (const Dot& dot: dots){
        center += dot;
    }
    center/=dots.size();
}
void Polygon::addDot(const Dot& dot){
    dots.push_back(dot);
    update();
}
qreal Polygon::getDepth(const DrawReq& drawReq) const{
    if (drawReq.projection == Central || drawReq.projection == Perspective){
        return getDistance(drawReq.viewer);
    }
    else{
        return getDistance(drawReq.onViewer);
    }
}
qreal Polygon::getDistance(const Vector& onViewer) const{
    return -onViewer*(center-Dot());
}
qreal Polygon::getDistance(const Dot& viewer) const{
    return (center-viewer).getModule();
}
QVector<Dot> Polygon::getDots() const{
    return dots;
}
bool Polygon::isConvexDot(int dotNum) const{
    Dot prev = dots[(dotNum-1+dots.size())%dots.size()];
    Dot cur = dots[dotNum];
    Dot next = dots[(dotNum+1)%dots.size()];

    Vector toCur = cur - prev;
    Vector fromCur = next - cur;

    return (toCur^fromCur).getY() > 0;
}
Composite* Polygon::getTringulation() const{
    Composite* triangles = new Composite();

    struct DotData{
        Dot dot;

        DotData* next;
        DotData* prev;
    };


    QVector<DotData*> data(dots.size());
    for (int i=0;i<dots.size();i++){
        data[i] = new DotData();
    }
    for (int i=0;i<dots.size();i++){
        data[i]->prev = data[(i-1+data.size())%data.size()];
        data[i]->dot = dots[i];
        data[i]->next = data[(i+1)%data.size()];
    }

    DotData* curData = data[0];
    int cnt = data.size();

    while (cnt >= 3){
        Dot prevDot = curData->prev->dot;
        Dot curDot = curData->dot;
        Dot nextDot = curData->next->dot;

        Vector toCur = curDot - prevDot;
        Vector fromCur = nextDot - curDot;

        if ((toCur^fromCur).getY() < 0){
            curData = curData->next;
            continue;
        }
        if (cnt == 3){
            triangles->addShape(new Polygon({prevDot, curDot, nextDot}, borderColor));
            break;
        }

        Vector a = prevDot - curDot;
        Vector b = nextDot - curDot;

        bool hasInnerDots = false;
        for (DotData* otherData = curData->next->next; otherData != curData->prev; otherData = otherData->next){
            if (otherData->dot == prevDot || otherData->dot == curDot || otherData->dot == nextDot){
                continue;
            }
            Vector toUncut = otherData->dot - curDot;

            matrix<2,3> belongMat({{a.getZ(), b.getZ(), toUncut.getZ()}
                                    ,{a.getX(), b.getX(), toUncut.getX()}});

            belongMat.makeEqualE();

            qreal k1 = belongMat[0][2],k2 = belongMat[1][2];

            if (k1 >= 0 && k2 >= 0 && k1 + k2 <= 1){
                hasInnerDots = 1;
                break;
            }
        }
        if (hasInnerDots){
            curData = curData->next;
            continue;
        }

        triangles->addShape(new Polygon({prevDot, curDot, nextDot}, borderColor));

        DotData* prev = curData->prev;
        DotData* next = curData->next;
        curData->prev->next = next;
        curData->next->prev = prev;

        curData = next;
        cnt--;

    }
    for (DotData* dotData: data){
        delete dotData;
    }
    return triangles;
}
Vector Polygon::getNormal() const{
    Vector res;
    int i = 0;
    do{
        if (i+2 >= dots.size()){
            throw std::invalid_argument("getNormal bad Polygon");
        }
        res = (dots[i+1] - dots[i])^(dots[i+2] - dots[i+1]);
        i++;
    } while(res.getModule() == 0);
    return res;
}
void Polygon::setBorderColor(const QColor& color_){
    borderColor = color_;
    if (!isBorder){
        borderOn();
    }
}
void Polygon::borderOff(){
    isBorder = false;
}
void Polygon::borderOn(){
    isBorder = true;
}
void Polygon::setBorderState(bool state){
    isBorder = state;
}
void Polygon::setBackColorState(bool state){
    isBackColor = state;
}

void Polygon::setColor(const QColor& color_){
    for (Dot& dot: dots){
        dot.setColor(color_);
    }
    update();
}
QVector<Polygon> Polygon::extractPolygons() const{
    return {*this};
}
Dot Polygon::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }

    return center;
}
Polygon Polygon::transformed(const matrix<4,4>& F) const{
    Polygon res(*this);
    res.transform(F);
    return res;
}
void Polygon::transform(const matrix<4,4>& F){
    Shape::transform(F);
    for (Dot& dot: dots){
        dot.transform(F);
    }
    update();
}
void Polygon::draw(QVector<QPointF> points, const DrawReq& drawReq) const{
    QWidget* context = drawReq.context;
    qreal zoom = drawReq.zoom;
    qreal koef = getKoef(context);
    QPainter p(context);

    for (QPointF& point: points){
        point.setX(context->width()/2+point.x()*koef/2*zoom);
        point.setY(context->height()/2-point.y()*koef/2*zoom);
    }
    Vector onViewer;
    if (drawReq.projection == Central || drawReq.projection == Perspective){
        onViewer = drawReq.viewer - getCenter();
    }
    else{
        onViewer = drawReq.onViewer;
    }


    bool isBack = false;

    //dots[0].show();
    //dots[1].show();
    //dots[2].show();
    //dots[3].show();
    //qDebug();
    if (onViewer * getNormal() < 0){
        isBack = true;
    }

    // COLOR
    QColor color;
    if (isBack && isBackColor){
        color = backColor;
    }
    else{
        color = center.getColor();
    }

    if (drawReq.useLight){
        qreal rLight = 0;
        qreal gLight = 0;
        qreal bLight = 0;

        for (const Vector& light: drawReq.lights){
            rLight += std::max( light.normalized() * (isBack?-getNormal():getNormal()).normalized() , 0.);
            gLight += std::max( light.normalized() * (isBack?-getNormal():getNormal()).normalized() , 0.);
            bLight += std::max( light.normalized() * (isBack?-getNormal():getNormal()).normalized() , 0.);
        }
        for (const QPair<Dot, qreal>& light: drawReq.lightDots){
            qreal energy;
            energy = std::max( (1 - (light.first-getCenter()).getModule() / light.second)
                *(light.first-getCenter()).normalized() * (isBack?-getNormal():getNormal()).normalized() , 0.);
            rLight += energy * light.first.getColor().redF();
            gLight += energy * light.first.getColor().greenF();
            bLight += energy * light.first.getColor().blueF();
        }

        color.setRedF(color.redF() * rLight);
        color.setGreenF(color.greenF() * gLight);
        color.setBlueF(color.blueF() * bLight);
    }
    p.setBrush(color);
    if (isBorder){
        p.setPen(borderColor);
    }
    else{
        p.setPen(p.brush().color());
    }
    p.drawPolygon(points);
    p.end();
}
void Polygon::draw(const DrawReq& drawReq) const{
    QVector<QPointF> pointsTemp;
    QVector<qreal> state;
    for (int i=0;i<dots.size();i++){

        matrix<4,1, qreal> mDot({
            {dots[i].getX()},
            {dots[i].getY()},
            {dots[i].getZ()},
            {1},
        });
        mDot = drawReq.F * mDot;

        state.push_back(mDot[3][0]);
        if (mDot[3][0] != 0){
            mDot /= mDot[3][0];
        }
        pointsTemp.push_back(Shape::normalizeByPlato(mDot[0][0],mDot[1][0],mDot[2][0],drawReq.plato));
    }
    QVector<QPointF> points;
    for (int i=0;i<pointsTemp.size();i++){

        if (state[i] <= 0){
            continue;
        }

        int iNext = (i+1)%pointsTemp.size();
        int iPrev = (i-1+pointsTemp.size())%pointsTemp.size();


        if (state[iPrev] == 0){
            points.push_back(Dot::extended(pointsTemp[i].x(),pointsTemp[i].y(), pointsTemp[iPrev].x(),pointsTemp[iPrev].y(), drawReq));
        }
        if (state[iPrev] < 0){
            points.push_back(Dot::reverse(pointsTemp[i].x(),pointsTemp[i].y(), pointsTemp[iPrev].x(),pointsTemp[iPrev].y(), drawReq));
        }

        points.push_back(pointsTemp[i]);

        if (state[iNext] == 0){
            points.push_back(Dot::extended(pointsTemp[i].x(),pointsTemp[i].y(),pointsTemp[iNext].x(),pointsTemp[iNext].y(), drawReq));
        }
        if (state[iNext] < 0){
            points.push_back(Dot::reverse(pointsTemp[i].x(),pointsTemp[i].y(),pointsTemp[iNext].x(),pointsTemp[iNext].y(), drawReq));
        }
    }
    draw(points, drawReq);
}
Polygon* Polygon::createCopy() const{
    return new Polygon(*this);
}

Polygon& Polygon::operator=(const Polygon& obj){
    if (&obj != this){
        copy(obj);
    }

    return *this;
}


#include "shape.h"

Shape::Shape(QColor clr) {
    setColor(clr);
    name = "NULL";
}
Shape::Shape(const Shape* obj){
    copy(obj);
}
Shape::~Shape(){

}
QString Shape::getName() const{
    return name;
}
QColor Shape::getColor() const{    
    return clr;
}
void Shape::setColor(const QColor& clr){
    this->clr = clr;
}

bool Shape::build(QPoint){
    return true;
}
void Shape::copy(const Shape* obj){
    setColor(obj->getColor());
}


Dot::Dot(QColor clr) : Shape(clr){
    name = "Точка";
}
Dot::Dot(int x, int y, QColor clr): Dot(clr){
    this->x = x;
    this->y = y;
}
Dot::Dot(QPoint pos, QColor clr):Dot(pos.x(),pos.y(),clr){

}
Dot::Dot(const Dot* obj): Dot(){
    copy(obj);
}

void Dot::paint(DrawReq& drawReq){
    QPainter p(drawReq.context);

    p.setBrush(clr);
    p.setPen(Qt::NoPen);
    QSize screenCntr = drawReq.context->size();
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();
    bool ask = drawReq.isAskew;

    p.drawRect(screenCntr.width()/2 +(-.5*ask+x+drawReq.xCntr)*scaleKoef + drawReq.xAskew -1,
               screenCntr.height()/2+(-1+.5*ask-y+drawReq.yCntr)*scaleKoef + drawReq.yAskew-1,
               1*scaleKoef+2,1*scaleKoef+2);

}
bool Dot::build(QPoint pos){
    x = pos.x();
    y = pos.y();
    return true;
}
Dot* Dot::createClearObj(){
    return new Dot();
}
void Dot::copy(const Dot* obj){
    Shape::copy(obj);
    this->x = obj->x;
    this->y = obj->y;
}
Dot* Dot::createCopy(){
    return new Dot(this);
}
QPoint Dot::getMinPoint() const{
    return {x,y};
}
QPoint Dot::getMaxPoint() const{
    return {x,y};
}
void Dot::exportImage(ExpReq& expReq) const{
    expReq.img.setPixelColor( x-expReq.start.x(),expReq.end.y()- y - 1, clr);
}

Line::Line(QColor clr): Shape(clr){
    step = 0;
    name = "Line";
}

Line::Line(int x0,int y0,int x1,int y1, QColor clr): Line(clr){
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
    step = 2;
}
Line::~Line(){
    for (Dot* dot: dots){
        delete dot;
    }
}

void Line::paint(DrawReq& drawReq){

    for (Dot* dot: dots){
        dot->paint(drawReq);
    }
}
bool Line::build(QPoint pos){
    if (step == 0){
        step++;
        x0 = pos.x();
        y0 = pos.y();
        return false;
    }
    else if (step == 1){
        step++;
        x1 = pos.x();
        y1 = pos.y();
        buildDots();
        return true;
    }
    return true;
}
void Line::copy(const Line* obj){
    Shape::copy(obj);
    this->dots = obj->dots;
    this->x0 = obj->x0;
    this->y0 = obj->y0;
    this->x1 = obj->x1;
    this->y1 = obj->y1;
    this->step = obj->step;
}
QPoint Line::getMinPoint() const{
    QPoint p = dots[0]->getMinPoint();
    int x = p.x(),y = p.y();

    for (const Dot* dot: dots){
        p = dot->getMinPoint();
        x = qMin(x, p.x());
        y = qMin(y, p.y());
    }
    return {x,y};
}
QPoint Line::getMaxPoint() const{
    QPoint p = dots[0]->getMaxPoint();
    int x = p.x(),y = p.y();

    for (const Dot* dot: dots){
        p = dot->getMaxPoint();
        x = qMax(x, p.x());
        y = qMax(y, p.y());
    }
    return {x,y};

}
void Line::exportImage(ExpReq& expReq) const{
    for (const Dot* dot: dots){
        dot->exportImage(expReq);
    }
}
LineDDA::LineDDA(QColor clr): Line(clr){
    name = "Линия DDA";
}
LineDDA::LineDDA(int x0,int y0,int x1,int y1, QColor clr): Line(x0,y0,x1,y1,clr){
    buildDots();
    name = "Линия DDA";
}
LineDDA::LineDDA(const LineDDA* obj): LineDDA(){
    copy(obj);
}

void LineDDA::buildDots(){
    int dx = x1-x0;
    int dy = y1-y0;

    int n = qMax(qAbs(dx),qAbs(dy));

    for (int t=0;t<=n;t++){
        int x = qRound(x0+(qreal)t/n*dx);
        int y = qRound(y0+(qreal)t/n*dy);
        dots.push_back(new Dot(x,y,clr));
    }
}

LineDDA* LineDDA::createClearObj(){
    return new LineDDA();
}
void LineDDA::copy(const LineDDA* obj){
    Line::copy(obj);
}
LineDDA* LineDDA::createCopy(){
    return new LineDDA(this);
}

LineBresenham::LineBresenham(QColor clr): Line(clr){
    name = "Линия Брезенхем";
}
LineBresenham::LineBresenham(int x0,int y0,int x1,int y1, QColor clr): Line(x0,y0,x1,y1,clr){
    buildDots();
    name = "Линия Брезенхем";
}
LineBresenham::LineBresenham(const LineBresenham* obj): LineBresenham(){
    copy(obj);
}

QString LineBresenham::getCode() const{
    int dx = x1-x0;
    int dy = y1-y0;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx < dy) qSwap(dx,dy);
// dx>=dy>=0
    QString code = "";
    int d = 2*dy-dx;
    if (d < 0)
        code += "S";
    else
        code += "D";

    for (int i=1;i<dx;i++){
        if (d<0){
            d+=2*dy;
        }
        else{
            d+=2*(dy-dx);
        }
        if (d < 0)
            code += "S";
        else
            code += "D";
    }
    return code;
}

void LineBresenham::buildDots(){

    int sdx,sdy,ddx,ddy;
    int dx = x1-x0;
    int dy = y1-y0;
    if (dx > dy && dy >= 0){
        sdx = 1;
        sdy = 0;
        ddx = 1;
        ddy = 1;
    }
    else if (dy >= dx && dx>0){
        sdx = 0;
        sdy = 1;
        ddx = 1;
        ddy = 1;
    }
    else if (dx<=0 && dy > -dx){
        sdx = 0;
        sdy = 1;
        ddx = -1;
        ddy = 1;
    }
    else if (dy>0 && dy <= -dx){
        sdx = -1;
        sdy = 0;
        ddx = -1;
        ddy = 1;
    }
    else if (dy <= 0 && dy > dx){
        sdx = -1;
        sdy = 0;
        ddx = -1;
        ddy = -1;
    }
    else if (dx < 0 && dy <= dx){
        sdx = 0;
        sdy = -1;
        ddx = -1;
        ddy = -1;
    }
    else if (dx>=0 && dy < -dx){
        sdx = 0;
        sdy = -1;
        ddx = 1;
        ddy = -1;
    }
    else if (dy<0 && dy >= -dx){
        sdx = 1;
        sdy = 0;
        ddx = 1;
        ddy = -1;
    }
    QString code = getCode();
    int x = x0, y = y0;
    dots.push_back(new Dot(x, y, clr));
    for (int i=0;i<code.size();i++){
        if (code[i] == 'S'){
            x += sdx;
            y += sdy;
        }
        else{
            x += ddx;
            y += ddy;
        }
        dots.push_back(new Dot(x, y, clr));
    }
}
LineBresenham* LineBresenham::createClearObj(){
    return new LineBresenham();
}
void LineBresenham::copy(const LineBresenham* obj){
    Line::copy(obj);
}
LineBresenham* LineBresenham::createCopy(){
    return new LineBresenham(this);
}
QString LineCastla::getCode() const{
    int dx = x1-x0;
    int dy = y1-y0;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx < dy) qSwap(dx,dy);
    // dx>=dy>=0
    QString code = "";

    QString m1 = "S",m2 = "D";

    int DX = dx;
    dx -= dy;

    if (dy == 0){
        for (int i=0;i<dx;i++){
            code += "S";
        }
        return code;
    }
    if (dx == 0){
        for (int i=0;i<DX;i++){
            code += "D";
        }
        return code;

    }
    while (dx!=dy && dx*dy){
        if (dy>dx){
            QString m1Rev = m1;
            std::reverse(m1Rev.begin(), m1Rev.end());
            m1 = m2 + m1Rev;
            dy -= dx;
        }
        else if (dx > dy){
            QString m2Rev = m2;
            std::reverse(m2Rev.begin(), m2Rev.end());
            m2 = m1 + m2Rev;
            dx -= dy;
        }
    }
    /// dx==dy;
    QString m2Rev = m2;
    std::reverse(m2Rev.begin(), m2Rev.end());
    m2 = m1 + m2Rev;

    for (int i=0;i<dx;i++){
        code+=m2;
    }

    return code;
}
void LineCastla::copy(const LineCastla* obj){
    Line::copy(obj);
}

void LineCastla::buildDots() {
    int sdx,sdy,ddx,ddy;
    int dx = x1-x0;
    int dy = y1-y0;
    if (dx > dy && dy >= 0){
        sdx = 1;
        sdy = 0;
        ddx = 1;
        ddy = 1;
    }
    else if (dy >= dx && dx>0){
        sdx = 0;
        sdy = 1;
        ddx = 1;
        ddy = 1;
    }
    else if (dx<=0 && dy > -dx){
        sdx = 0;
        sdy = 1;
        ddx = -1;
        ddy = 1;
    }
    else if (dy>0 && dy <= -dx){
        sdx = -1;
        sdy = 0;
        ddx = -1;
        ddy = 1;
    }
    else if (dy <= 0 && dy > dx){
        sdx = -1;
        sdy = 0;
        ddx = -1;
        ddy = -1;
    }
    else if (dx < 0 && dy <= dx){
        sdx = 0;
        sdy = -1;
        ddx = -1;
        ddy = -1;
    }
    else if (dx>=0 && dy < -dx){
        sdx = 0;
        sdy = -1;
        ddx = 1;
        ddy = -1;
    }
    else if (dy<0 && dy >= -dx){
        sdx = 1;
        sdy = 0;
        ddx = 1;
        ddy = -1;
    }
    QString code = getCode();
    int x = x0, y = y0;
    dots.push_back(new Dot(x, y, clr));
    for (int i=0;i<code.size();i++){
        if (code[i] == 'S'){
            x += sdx;
            y += sdy;
        }
        else{
            x += ddx;
            y += ddy;
        }
        dots.push_back(new Dot(x, y, clr));
    }
}

LineCastla::LineCastla(QColor clr):Line(clr){
    name = "Линия Кастла-Питвея";
}
LineCastla::LineCastla(int x0,int y0,int x1,int y1, QColor clr): Line(x0,y0,x1,y1){
    name = "Линия Кастла-Питвей";
}

LineCastla::LineCastla(const LineCastla* obj): LineCastla(){
    copy(obj);
}

LineCastla* LineCastla::createClearObj(){
    return new LineCastla();
}

LineCastla* LineCastla::createCopy(){
    return new LineCastla(this);
}

CircleBresenham::CircleBresenham(QColor clr): Shape(clr){
    name = "Окружность Брезенхем";
}

CircleBresenham::CircleBresenham(int x0,int y0,int r, QColor clr): CircleBresenham(clr){
    this->x0 = x0;
    this->y0 = y0;
    this->r = r;
    buildDots();
}

CircleBresenham::CircleBresenham(const CircleBresenham* obj): CircleBresenham(){
    copy(obj);
}

CircleBresenham::~CircleBresenham(){
    for (Dot* dot: dots){
        delete dot;
    }
}

void CircleBresenham::copy(const CircleBresenham* obj){
    Shape::copy(obj);
    this->dots = obj->dots;
    this->x0 = obj->x0;
    this->y0 = obj->y0;
    this->r = obj->r;
    this->step = obj->step;
}

void CircleBresenham::buildDots(){
    int x = 0,y = r;
    QVector<QPoint> points;
    points.push_back({x,y});
    points.push_back({x,-y});
    points.push_back({-x,y});
    points.push_back({-x,-y});
    points.push_back({y,x});
    points.push_back({y,-x});
    points.push_back({-y,x});
    points.push_back({-y,-x});

    int D = 2-2*r;
    while (x<y){

        if (D < 0){
            int dMinus = 2*D+2*y-1;
            if (dMinus < 0){
                x+=1;
                D = D + 2*x+1;
            }
            else{
                x+=1;
                y-=1;
                D = D + 2*x - 2*y+2;
            }
        }
        else{
            int dPlus = 2*D -2*x - 1;
            if (dPlus < 0){
                x+=1;
                y-=1;
                D = D +2*x-2*y+2;
            }
            else{
                qDebug()<<"dPlus positive - impossible!";
            }
        }

        points.push_back({x,y});
        points.push_back({x,-y});
        points.push_back({-x,y});
        points.push_back({-x,-y});
        points.push_back({y,x});
        points.push_back({y,-x});
        points.push_back({-y,x});
        points.push_back({-y,-x});

    }


    for (QPoint point: points){
        dots.push_back(new Dot(point.x()+x0,point.y()+y0, clr));
    }
}

bool CircleBresenham::build(QPoint pos){
    if (step == 0){
        step++;
        x0 = pos.x();
        y0 = pos.y();
        return false;
    }
    else if (step == 1){
        step++;
        r = qRound(std::hypot(pos.x()-x0, pos.y()-y0));
        buildDots();
        return true;
    }
    return true;
}
void CircleBresenham::paint(DrawReq& drawReq){
    for (Dot* dot: dots){
        dot->paint(drawReq);
    }
}

CircleBresenham* CircleBresenham::createClearObj(){
    return new CircleBresenham();
}
CircleBresenham* CircleBresenham::createCopy(){
    return new CircleBresenham(this);
}
QPoint CircleBresenham::getMinPoint() const{
    QPoint p = dots[0]->getMinPoint();
    int x = p.x(),y = p.y();

    for (const Dot* dot: dots){
        p = dot->getMinPoint();
        x = qMin(x, p.x());
        y = qMin(y, p.y());
    }
    return {x,y};
}
QPoint CircleBresenham::getMaxPoint() const{
    QPoint p = dots[0]->getMaxPoint();
    int x = p.x(),y = p.y();

    for (const Dot* dot: dots){
        p = dot->getMaxPoint();
        x = qMax(x, p.x());
        y = qMax(y, p.y());
    }
    return {x,y};

}
void CircleBresenham::exportImage(ExpReq& expReq) const{
    for (const Dot* dot: dots){
        dot->exportImage(expReq);
    }
}
void LineVu::copy(const LineVu* obj){
    Shape::copy(obj);
    this->x0 = obj->x0;
    this->y0 = obj->y0;
    this->x1 = obj->x1;
    this->y1 = obj->y1;
    this->dots = obj->dots;
    this->step= obj->step;
    this->N= obj->N;
    this->M= obj->M;
}
QPoint LineVu::turnOct(int x,int y, int oct){
    /// x>=y>=0;

    if (oct == 1){
        return {x,y};
    }
    else if (oct == 2){
        return {y,x};
    }
    else if (oct == 3){
        return {-y,x};
    }
    else if (oct == 4){
        return {-x,y};
    }
    else if (oct == 5){
        return {-x,-y};
    }
    else if (oct == 6){
        return {-y,-x};
    }
    else if (oct == 7){
        return {y,-x};
    }
    else if (oct == 8){
        return {x,-y};
    }
}

void LineVu::buildDots(){

    int D = 0;
    int dx = x1-x0;
    int dy = y1-y0;
    if (dx == 0 && dy == 0){
        dots.push_back(new Dot(x0,y0,clr));
        return;
    }
    if (dx == 0){
        for (int i=0;i<=qAbs(dy);i++){
            dots.push_back(new Dot(x0,y0+i*dy/qAbs(dy),clr));
        }
        return;
    }
    if (dy == 0){
        for (int i=0;i<=qAbs(dx);i++){
            dots.push_back(new Dot(x0+i*dx/qAbs(dx),y0,clr));
        }
        return;
    }
    if (qAbs(dx) == qAbs(dy)){
        for (int i=0;i<=qAbs(dy);i++){
            dots.push_back(new Dot(x0+i*dx/qAbs(dx),y0+i*dy/qAbs(dy),clr));
        }
        return;
    }
    int oct = 0;
    if (dx > dy && dy >= 0){
        oct = 1;
    }
    else if (dy >= dx && dx>0){
        oct = 2;
    }
    else if (dx<=0 && dy > -dx){
        oct = 3;
    }
    else if (dy>0 && dy <= -dx){
        oct = 4;
    }
    else if (dy <= 0 && dy > dx){
        oct = 5;
    }
    else if (dx < 0 && dy <= dx){
        oct = 6;
    }
    else if (dx>=0 && dy < -dx){
        oct = 7;
    }
    else if (dy<0 && dy >= -dx){
        oct = 8;
    }
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx<dy) qSwap(dx,dy);
    // dx>=dy>=0

    if (dy == 0 || dy>=dx){
        return;
    }

    int d = N*dy/dx;

    int x1=0,x2=dx,y1=0,y2=dy;

    QColor clr = this->getColor();
    dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x1,y1,oct),clr));
    dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x2,y2,oct),clr));

    while(1){
        x1+=1;
        x2-=1;
        if (x1>x2)
            return;
        D+=d;
        if(D>=N){
            D-=N;
            y1+=1;
            y2-=1;
        }
        qreal c =(qreal)D/N;
//        qreal c =(qreal)D/(N/M)/M;
        QColor clr1 = clr;
        clr1.setAlphaF(1-c);
        QColor clr2 = clr;
        clr2.setAlphaF(c);
        dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x1,y1,oct), clr1));
        dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x2,y2,oct), clr1));
        if (!(x1==x2 && y1 == y2-1))
            dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x2,y2-1,oct), clr2));
        if (!(x1==x2 && y2 == y1-1))
            dots.push_back(new Dot(QPoint(x0,y0)+turnOct(x1,y1+1,oct), clr2));

    }
}

LineVu::LineVu(QColor clr):Line(clr){
    name = "Линия Ву";
}
LineVu::LineVu(int x0,int y0,int x1, int y1, QColor clr): Line(x0,y0,x1,y1,clr){
    name = "Линия Ву";
}
LineVu::LineVu(const LineVu* obj):LineVu(){
    copy(obj);
}


LineVu* LineVu::createClearObj(){
    return new LineVu();
}
LineVu* LineVu::createCopy(){
    return new LineVu(this);
}

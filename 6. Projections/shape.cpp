#include "shape.h"
void Shape::copy(const Shape& obj){
    setName(obj.getName());
    if (obj.isCenterSet){
        setCenter(Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]));
    }
    animations = obj.animations;
    animationSteps = obj.animationSteps;
    animationGlobalDelay = obj.animationGlobalDelay;
    animationLifeTime = obj.animationLifeTime;
}
Shape::Shape(){
    animationGlobalDelay = 0;

    isCenterSet = false;
//    qDebug()<<typeid(this).name()<<"CREATED";
}
Shape::Shape(const Shape& obj): Shape(){
    copy(obj);
}
Shape::~Shape(){
//    qDebug()<<typeid(this).name()<<name<<"DESTRYED";
}
void Shape::setName(const QString& name_){
    name = name_;
}
QString Shape::getName() const{
    return name;
}
void Shape::addAnimation(void (*animation)(Shape*, int), int delay, int lifetime){
    animations.push_back(animation);
    animationSteps.push_back(-delay);
    animationLifeTime.push_back(lifetime);
}
void Shape::setAnimationGlobalDelay(int delay_){
    animationGlobalDelay = delay_;
}

void Shape::animate(){
    if (animationGlobalDelay){
        animationGlobalDelay--;
        return;
    }
    for (int i=0;i<animations.size();i++){
        if (animationSteps[i] < 0){
            animationSteps[i]++;
            continue;
        }
        if (animationLifeTime[i] == 0){
            animations.remove(i);
            animationSteps.remove(i);
            animationLifeTime.remove(i);
            i--;
            continue;
        }
        if (animationLifeTime[i] > 0){
            animationLifeTime[i]--;
        }
        animations[i](this, animationSteps[i]++);
        animationSteps[i] %= 1024 * 3 * 5;
    }
}
matrix<4,4> Shape::getLocalTransformation(matrix<4,4> F) const{
    Dot center = getCenter();

    matrix<4,4, qreal> toCenter({
            {1,0,0,-center.getX()},
            {0,1,0,-center.getY()},
            {0,0,1,-center.getZ()},
            {0,0,0,1},
    });
    matrix<4,4, qreal> fromCenter({
            {1,0,0,center.getX()},
            {0,1,0,center.getY()},
            {0,0,1,center.getZ()},
            {0,0,0,1},
    });
    return fromCenter*F*toCenter;
}
void Shape::rotateX(qreal angle, bool isLocal){
    matrix<4,4, qreal> F({
        {1,0,0,0},
        {0,cos(angle),-sin(angle),0},
        {0,sin(angle), cos(angle),0},
        {0,0,0,1},
    });
    if (isLocal){
        F = getLocalTransformation(F);
    }
    transform(F);
}
void Shape::rotateXEuler(qreal angle, bool isLocal){
    rotateX(angle*M_PI/180, isLocal);
}
void Shape::rotateY(qreal angle, bool isLocal){
    matrix<4,4, qreal> F({
       { cos(angle), 0, sin(angle),0},
        {0,1,0,0},
       {-sin(angle), 0, cos(angle),0},
       {0,0,0,1},
    });
    if (isLocal){
        F = getLocalTransformation(F);
    }
    transform(F);
}
void Shape::rotateYEuler(qreal angle, bool isLocal){
    rotateY(angle*M_PI/180, isLocal);
}
void Shape::rotateZ(qreal angle, bool isLocal){
    matrix<4,4, qreal> F({
        {cos(angle),-sin(angle),0,0},
        {sin(angle), cos(angle),0,0},
        {0,0,1,0},
        {0,0,0,1},
    });
    if (isLocal){
        F = getLocalTransformation(F);
    }
    transform(F);
}
void Shape::rotateZEuler(qreal angle, bool isLocal){
    rotateZ(angle*M_PI/180, isLocal);
}
void Shape::move(qreal dx, qreal dy, qreal dz){
    matrix<4,4, qreal> F({
       {1,0,0,dx},
       {0,1,0,dy},
       {0,0,1,dz},
       {0,0,0,1},
    });
    transform(F);
}
void Shape::move(Vector vec){
    move(vec.getX(),vec.getY(),vec.getZ());
}
void Shape::scale(qreal dx, qreal dy, qreal dz, bool isLocal){
    matrix<4,4, qreal> F({
       {dx,0,0,0},
       {0,dy,0,0},
       {0,0,dz,0},
       {0,0,0,1},
    });
    if (isLocal){
        F = getLocalTransformation(F);
    }
    transform(F);
}
void Shape::setCenter(Dot center_){
    isCenterSet = true;
    mCenter[0][0] = center_.getX();
    mCenter[1][0] = center_.getY();
    mCenter[2][0] = center_.getZ();
    mCenter[3][0] = 1;
}
void Shape::resetCenter(){
    isCenterSet = false;
}
bool Shape::getCenterSet() const{
    return  isCenterSet;
}

QPointF Shape::normalizeByPlato(qreal x, qreal y, qreal z, Plato plato){
    if (plato == Oxy){
        return {x,y};
    }
    if (plato == Oyz){
        return {-z,y};
    }
    if (plato == Ozx){
        return {x,-z};
    }
    qDebug()<<"Bad plato!";
    throw 1;
    return {-1,-1};
}
int Shape::getKoef(QWidget* context){
    return std::min(context->width(),context->height());
}
QVector<Polygon> Shape::extractPolygons() const{
    return QVector<Polygon>();
}
void Shape::transform(const matrix<4,4, qreal>& F){
    if (!isCenterSet){
        return;
    }
    mCenter = F * mCenter;
    if (mCenter[3][0] == 0){
        throw std::invalid_argument("bad not affin transformation");
    }
    mCenter /= mCenter[3][0];
}

Shape& Shape::operator=(const Shape& obj){
    if (&obj != this){
        copy(obj);
    }
    return *this;
}


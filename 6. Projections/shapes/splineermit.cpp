#include "shape.h"
QVector<qreal> getStraightQVector(qreal a, qreal b){
    QVector<qreal> res;
    for (qreal i=a;i<b;i++){
        res.push_back(i);
    }
    return res;
}
SplineErmit::SplineErmit():Spline(), approximation(-1){ }
SplineErmit::SplineErmit(const QVector<Dot>& us_): SplineErmit(us_, getStraightQVector(0, us_.size())){ }
SplineErmit::SplineErmit(const QVector<Dot>& us_, const QVector<qreal>& ts_): SplineErmit(){
    for (int i=0; i<std::min(us_.size(), ts_.size()); i++){
        addNode(us_[i], ts_[i]);
    }
}
SplineErmit::SplineErmit(const SplineErmit& obj): SplineErmit(){
    (*this) = obj;
}
SplineErmit::~SplineErmit(){
    clearNodes();
}

void SplineErmit::updateKoefs(){
    int N = ts.size();
    int M = 0;
    QVector<qreal> vts;

    for (int i=0;i<vs.size();i++){
        if (vs[i] != Vector(0,0,0)){
            M++;
            vts.push_back(ts[i]);
        }
    }
    matrix_light<qreal> Q((approximation==-1?N+M:approximation), N+M);
    for (int j=0;j<N;j++){
        Q[0][j] = 1;
    }
    for (int i=1;i<Q.getN();i++){
        for (int j=0;j<N;j++){
            Q[i][j] = Q[i-1][j] * ts[j];
        }
    }
    for (int j=0;j<M;j++){
        Q[0][j+N] = 0;
        Q[1][j+N] = 1;
    }
    for (int i=2;i<Q.getN();i++){
        for (int j=0;j<M;j++){
            Q[i][j+N] = Q[i-1][j+N] / (i-1) * vts[j] * i;
        }
    }
    matrix_light<qreal> H(Q.getM(), Q.getN());
    if (Q.getN() == Q.getM()){
        H = Q.inverted();
    }
    else if (Q.getN() < Q.getM()){
        H = Q.transposed() * ( Q * Q.transposed() ).inverted();
    }
    else{
        H = ( Q.transposed() * Q ).inverted() * Q.transposed();
    }
    ls.clear();
    for (int i=0;i<H.getN();i++){
        ls.push_back(Polynom(H.getRow(i)));
    }
    updateSegments();
}
Dot SplineErmit::getDot(qreal t) const{
    Dot res(0,0,0);
    qreal r = 0,g = 0,b = 0;
    QVector<Vector> curVs;
    for (int i=0;i<vs.size();i++){
        if (vs[i] != Vector(0,0,0)){
            curVs.push_back(vs[i]);
        }
    }
    for (int i=0;i<us.size();i++){
        qreal l = ls[i].getVal(t);
        res += us[i]*l;
        QColor color_ = us[i].getColor();
        r += color_.red() * l;
        g += color_.green() * l;
        b += color_.blue() * l;
    }
    for (int i=us.size();i<ls.size();i++){
        res += curVs[i-us.size()]*ls[i].getVal(t);
    }
    res.setColor(QColor(r, g, b));
    return res;
}

void SplineErmit::setApproximation(int approximation_){
    approximation = approximation_;
    updateKoefs();
}
int SplineErmit::getApproximation() const{
    return approximation;
}
qreal SplineErmit::getTEnd() const{
    if (ts.size() == 0){
        return 0;
    }
    return ts[ts.size()-1];
}

void SplineErmit::addNode(const Dot& u_, const Vector& v_){
    qreal t_;
    if (ts.size() == 0){
        t_ = 0;
    }
    else if (ts.size() == 1){
        t_ = ts[0]+1;
    }
    else{
        t_ = ts[ts.size()-1] + (ts[ts.size()-1] - ts[0])/(ts.size()-1);
    }
    addNode(u_, v_, t_);
}
void SplineErmit::addNode(const Dot& u_, qreal t_){
    addNode(u_, Vector(0,0,0), t_);
}
void SplineErmit::addNode(const Dot& u_, const Vector& v_, qreal t_){
    us.push_back(u_);
    ts.push_back(t_);
    vs.push_back(v_);
    updateKoefs();
}

void SplineErmit::setNode(int i, const Dot& u_){
    setNode(i, u_, ts[i]);
}
void SplineErmit::setNode(int i, const Dot& u_, qreal t_){
    setNode(i, u_, vs[i], t_);
}
void SplineErmit::setNode(int i, const Dot& u_, const Vector& v_){
    setNode(i, u_, v_, ts[i]);
}
void SplineErmit::setNode(int i, const Dot& u_, const Vector& v_, qreal t_){
    us[i] = u_;
    vs[i] = v_;
    ts[i] = t_;
    updateKoefs();
}

void SplineErmit::clearNodes(){
    us.clear();
    ts.clear();
    vs.clear();
}
void SplineErmit::copy(const SplineErmit& obj){
    Spline::copy(obj);
    us = obj.us;
    ts = obj.ts;
    vs = obj.vs;
    ls = obj.ls;
    approximation = obj.approximation;
    updateSegments();
}
void SplineErmit::setVector(int i, const Vector& vec){
    vs[i] = vec;
    updateKoefs();
}
QVector<Dot> SplineErmit::getDots() const{
    return us;
}
QVector<Dot> SplineErmit::getUs() const{
    return getDots();
}
Vector SplineErmit::getVector(int i) const{
    return vs[i];
}
QVector<Vector> SplineErmit::getVectors() const{
    return vs;
}
QVector<qreal> SplineErmit::getTs() const{
    return ts;
}
void SplineErmit::setColor(const QColor& color){
    for (Dot& dot: us){
        dot.setColor(color);
    }
    updateSegments();
}

SplineErmit SplineErmit::transformed(const matrix<4,4>& F) const{
    SplineErmit res(*this);
    res.transform(F);
    return res;
}
void SplineErmit::transform(const matrix<4,4>& F){
    Spline::transform(F);
    for (int i=0;i<us.size();i++){
        us[i].transform(F);
        vs[i].transform((matrix<3,3>)F);
    }
    updateSegments();
}

SplineErmit* SplineErmit::createCopy() const{
    return new SplineErmit(*this);
}

SplineErmit& SplineErmit::operator=(const SplineErmit& obj){
    if (this != &obj){
        copy(obj);
    }
    return *this;
}

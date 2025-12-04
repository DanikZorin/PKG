#ifndef SHAPE_H
#define SHAPE_H
#include <QWidget>
#include <QPainter>
#include "matrix.h"
#include "matrix_light.h"
#include "vector.h"
#include "polynom.h"
#include <QLinearGradient>
enum Projection { Aksonometric, Askew, Central, Perspective };
enum Plato { Oxy = 1 + 2, Oyz = 2 + 4, Ozx = 1 + 4 };
struct DrawReq;
class Dot;
class Polygon;

QColor createColor(const matrix<3,1, qreal>& colorM);
class Shape {
protected:
    QString name;
    QVector<void(*)(Shape*, int)> animations;
    QVector<int> animationSteps;
    QVector<int> animationLifeTime;
    int animationGlobalDelay;

    matrix<4,1, qreal> mCenter;
    bool isCenterSet;

    void copy(const Shape& obj);
public:
    Shape();
    Shape(const Shape& obj);

    virtual ~Shape();
    void setName(const QString& name_);
    QString getName() const;
    void addAnimation(void (*animation)(Shape*, int), int delay = 0, int lifeTime = -1);
    void setAnimationGlobalDelay(int);
    void animate();

    matrix<4,4> getLocalTransformation(matrix<4,4> F) const;

    void rotateX(qreal angle, bool isLocal = 1);
    void rotateXEuler(qreal angle, bool isLocal = 1);
    void rotateY(qreal angle, bool isLocal = 1);
    void rotateYEuler(qreal angle, bool isLocal = 1);
    void rotateZ(qreal angle, bool isLocal = 1);
    void rotateZEuler(qreal angle, bool isLocal = 1);
    void move(qreal dx, qreal dy, qreal dz);
    void move(Vector vec);
    void scale(qreal dx, qreal dy, qreal dz, bool isLocal = 1);

    void setCenter(Dot center_);
    void resetCenter();
    bool getCenterSet() const;
    static QPointF normalizeByPlato(qreal, qreal, qreal, Plato);
    static int getKoef(QWidget*);

    virtual void setColor(const QColor& color) = 0;

    virtual QVector<Polygon> extractPolygons() const;
    virtual Dot getCenter() const = 0;
    virtual void transform(const matrix<4,4>& F);
    virtual void draw(const DrawReq& drawReq) const = 0;

    virtual Shape* createCopy() const = 0;

    Shape& operator=(const Shape&);    
};
class Composite;
class Dot: public Shape{
protected:
    qreal x,y,z;

    void copy(const Dot& obj);
    static QPointF extended(qreal x, qreal y, qreal dx, qreal dy, const DrawReq& drawReq);
    static QPointF reverse(qreal xReal, qreal yReal, qreal xFake, qreal yFake, const DrawReq& drawReq);
    void draw(qreal x_,qreal y_, const DrawReq& drawReq) const;
public:
    matrix<3,1, qreal> colorM;
    Dot();
    Dot(qreal x_, qreal y_, qreal z_, QColor = Qt::white);
    Dot(const Dot&);

    void setPoint(qreal x_, qreal y_, qreal z_);
    qreal getX() const;
    qreal getY() const;
    qreal getZ() const;
    void show() const;

    void setColor(const QColor& color) override;
    QColor getColor() const;

    Dot getCenter() const override;
    Dot transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;
    void draw(const DrawReq& drawReq) const override;

    Dot* createCopy() const override;

    Vector operator -(const Dot&) const;
    Dot operator +(const Vector&) const;
    Dot operator *(qreal) const;
    friend Dot operator*(qreal, const Dot);
    Dot operator +(const Dot&) const;
    Dot& operator +=(const Vector&);
    Dot& operator +=(const Dot&);
    Dot operator -(const Vector&) const;
    Dot& operator -=(const Vector&);
    Dot& operator *=(qreal);
    Dot operator /(qreal) const;
    Dot& operator /=(qreal);

    friend Dot operator *(matrix<4,4, qreal> F, const Dot& obj);


    Dot& operator =(const Dot&);

    bool operator ==(const Dot&) const;
    friend class Line;
    friend class Polygon;
};
class Line: public Shape{
protected:
    Dot begin;
    Dot end;

    static QPair<QPointF,QPointF> getLineScreened(const Dot& start, const Dot& end, const DrawReq& drawReq);

    void copy(const Line& obj);
    void draw(qreal x1, qreal y1, qreal x2, qreal y2, const DrawReq& drawReq) const;
public:
    Line();
    Line(const Dot&, const Dot&);
    Line(const Dot&, const Dot&, QColor color_);
    Line(const Line&);

    ~Line();

    void setBegin(qreal x, qreal y, qreal z);
    void setBegin(const Dot& begin_);
    void setBegin(const Dot* begin_);
    void setEnd(qreal x, qreal y, qreal z);
    void setEnd(const Dot& end_);
    void setEnd(const Dot* end_);

    Dot getBegin() const;
    Dot getEnd() const;

    void setColor(const QColor& color) override;

    Dot getCenter() const override;
    Line transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;
    void draw(const DrawReq& drawReq) const override;

    Line* createCopy() const override;

    Line& operator =(const Line&);

    friend class Polygon;
};
class Polygon: public Shape{
public:
    QVector<Dot> dots;
    Dot center;

    bool isBorder;
    QColor borderColor;

    bool isBackColor;
    QColor backColor;
    void copy(const Polygon& obj);
    void draw(QVector<QPointF> dots, const DrawReq& drawReq) const;
public:
    Polygon();
    Polygon(QColor color_);
    Polygon(const QVector<Dot>& dots_, QColor borderColor_ = Qt::black);
    Polygon(const Polygon&);

    ~Polygon();

    void update();

    void addDot(const Dot&);

    qreal getDepth(const DrawReq& drawReq) const;

    qreal getDistance(const Vector& vec) const;
    qreal getDistance(const Dot& dot) const;
    bool isConvexDot(int dotNum) const;
    Composite* getTringulation() const;

    QVector<Dot> getDots() const;

    Vector getNormal() const;

    void setBorderColor(const QColor& color_);
    void borderOff();
    void borderOn();
    void setBorderState(bool state);

    void setBackColorState(bool state);

    void setColor(const QColor& color) override;

    QVector<Polygon> extractPolygons() const override;
    Dot getCenter() const override;
    Polygon transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;
    void draw(const DrawReq& drawReq) const override;

    Polygon* createCopy() const override;

    Polygon& operator=(const Polygon&);

    friend class Polyedr;
};

class Polyedr: public Shape{
public:
    QVector<Polygon> polygons;
    Dot center;

    void copy(const Polyedr&);
public:
    Polyedr();
    Polyedr(const QVector<Polygon>&);
    Polyedr(const Polyedr&);
    ~Polyedr();

    void update();

    void addPolygon(const Polygon&);
    QVector<Polygon> getPolygons() const;
    Vector getNormalOut(const Polygon& polygon) const;
    bool isFacePolygon(const Polygon& polygon, const Vector& onViewer) const;
    static Polyedr expandLine(const Line& line, qreal extraWidth, Plato plato, QColor borderColor = Qt::black, bool expandMain = true);

    QVector<Polygon> extractPolygons() const override;
    Dot getCenter() const override;
    static Polyedr createPrism(const Dot& corner, Vector a, Vector b, Vector c, QColor color = Qt::black);

    void setColor(const QColor& color_) override;
    Polyedr transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;
    void draw(const DrawReq& drawReq) const override;

    Polyedr* createCopy() const override;

    Polyedr& operator=(const Polyedr&);

};
class Composite: public Shape{
public:
    QVector<Shape*> shapes;

    QVector<Polygon> extractedPolygons;
    void copy(const Composite&);
public:
    Composite();
    Composite(const QVector<Shape*>&);
    Composite(const Composite&);

    ~Composite();

    void update();

    void clearShapes();
    QVector<Shape*> getShapes() const;
    void addShape(const Shape *const);
    void addShapeDel(Shape*);

    QVector<qreal> getPolygonsDepth(const DrawReq& drawReq) const;

    QVector<Dot> extractDots() const;
    QVector<Dot> getConvexBoundDots() const;
    Polygon* getConvexBound() const;
    Composite* getTriangulationDelone() const;

    QVector<Polygon> extractPolygons() const override;
    void setColor(const QColor& color) override;
    Dot getCenter() const override;
    Composite transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;
    void drawByDrawman(const DrawReq& drawReq) const;
    void draw(const DrawReq& drawReq) const override;

    Composite* createCopy() const override;

    Composite& operator=(const Composite&);

};
class Spline: public Shape{
private:
    int subNodes;

    Composite segments;
protected:
    void copy(const Spline& obj);
public:
    Spline();
    virtual ~Spline();
    void updateSegments();
    virtual Dot getDot(qreal t) const = 0;

    virtual qreal getTEnd() const;
    void setSubNodes(int cnt_);

    virtual void transform(const matrix<4,4>& F) override;
    Dot getCenter() const override;
    void draw(const DrawReq& drawReq) const override;

    virtual Spline* createCopy() const override = 0;
};
class SplineKinematic: public Spline{
    matrix<4,4> Fpoints;
    Dot (*F)(Dot dot, qreal t);

    qreal tStart;
    qreal tEnd;
    Dot begin;

    void copy(const SplineKinematic& obj);
public:
    SplineKinematic(Dot begin_, Dot (*F_)(Dot dot, qreal t) = 0, qreal tEnd_ = 1);
    SplineKinematic(const SplineKinematic& obj);

    void setTEnd(qreal t_);
    void setTStart(qreal t_);

    Dot getDot(qreal t) const override;
    qreal getTEnd() const override;
    void setColor(const QColor& color) override;

    SplineKinematic transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;

    SplineKinematic* createCopy() const override;

    SplineKinematic& operator=(const SplineKinematic&);
};

class SplineErmit: public Spline{
protected:
    QVector<Dot> us;
    QVector<qreal> ts;
    QVector<Vector> vs;
    QVector<Polynom> ls;

    int approximation;
    void copy(const SplineErmit& obj);
public:
    SplineErmit();
    SplineErmit(const QVector<Dot>& us_);
    SplineErmit(const QVector<Dot>& us_, const QVector<qreal>& ts_);
    SplineErmit(const SplineErmit& obj);

    ~SplineErmit();

    void updateKoefs();

    Dot getDot(qreal t) const override;
    void setApproximation(int approximation_);
    int getApproximation() const;
    qreal getTEnd() const override;

    void addNode(const Dot& u_, const Vector& v_ = Vector(0,0,0));
    void addNode(const Dot& u_, qreal t_);
    void addNode(const Dot& u_, const Vector& v_, qreal t_);

    void setNode(int i, const Dot& u_);
    void setNode(int i, const Dot& u_, qreal t_);
    void setNode(int i, const Dot& u_, const Vector& v_);
    void setNode(int i, const Dot& u_, const Vector& v_, qreal t_);

    void clearNodes();

    void setVector(int i, const Vector& vec);

    QVector<Dot> getDots() const;
    QVector<Dot> getUs() const;

    Vector getVector(int i) const;
    QVector<Vector> getVectors() const;
    QVector<qreal> getTs() const;

    void setColor(const QColor& color) override;

    SplineErmit transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;

    SplineErmit* createCopy() const override;

    SplineErmit& operator=(const SplineErmit&);
};

class Surface: public Shape{
private:
    int subNodes1;
    int subNodes2;

    bool isBorder;
    bool isPolygonBack;
    QColor borderColor;
    Composite segments;
protected:

    void copy(const Surface& obj);
public:
    Surface();

    void updateSegments();
    virtual Dot getDot(qreal t1, qreal t2) const = 0;

    void setSubNodes(int cnt_);
    void setSubNodes1(int cnt_);
    void setSubNodes2(int cnt_);

    int getSubNodes1() const;
    int getSubNodes2() const;

    void setBorderColor(const QColor& color_);
    void borderOff();
    void borderOn();

    void polygonBackOff();
    void polygonBackOn();

    virtual qreal getTEnd1() const;
    virtual qreal getTEnd2() const;
    virtual void transform(const matrix<4,4>& F) override;

    QVector<Polygon> extractPolygons() const override;
    Dot getCenter() const override;
    void draw(const DrawReq& drawReq) const override;
};

class SurfaceErmit: public Surface{
    Dot u00;
    Dot u01;
    Dot u10;
    Dot u11;

    Vector v00, v01, v10, v11; // 1'st dir vec
    Vector w00, w01, w10, w11; // 2'nd dir vec
    Vector k00, k01, k10, k11; // rotation vec

    QVector<Polynom> ls;

    void copy(const SurfaceErmit& obj);
public:
    SurfaceErmit();
    SurfaceErmit(const Dot&, const Dot&, const Dot&, const Dot&, QColor = Qt::black);
    SurfaceErmit(const SurfaceErmit& obj);

    ~SurfaceErmit();

    Dot getDot(qreal t1, qreal t2) const override;

    void setDot(int i, const Dot& u);
    void setVecDir1(int i, const Vector& v);
    void setVecDir2(int i, const Vector& w);
    void setVecRot(int i, const Vector& k);
    void setNode(int i, const Dot& u, const Vector& v, const Vector& w, const Vector& k);

    Dot getDot(int i) const;
    Vector getVecDir1(int i) const;
    Vector getVecDir2(int i) const;
    Vector getVecRot(int i) const;

    void setColor(const QColor& color) override;

    SurfaceErmit transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;

    SurfaceErmit* createCopy() const override;


    SurfaceErmit& operator =(const SurfaceErmit& obj);
};
class SurfaceInterpolated: public Surface{
    QVector<QVector<Dot>> us;
    QVector<QVector<Vector>> vs;
    QVector<QVector<Vector>> ws;
    Vector k0,k1,k2,k3;

    QVector<Polynom> ls1;
    QVector<Polynom> ls2;

    void copy(const SurfaceInterpolated& obj);
public:
    SurfaceInterpolated();
    SurfaceInterpolated(const QVector<QVector<Dot>>& us_, QColor = Qt::black);
    SurfaceInterpolated(const SurfaceInterpolated& obj);

    ~SurfaceInterpolated();

    void updateKoefs();

    Dot getDot(qreal t1, qreal t2) const override;

    void setDot(int i, int j, const Dot& u);
    void setVecDir1(int i, int j, const Vector& v);
    void setVecDir2(int i, int j, const Vector& w);
    void setVecRot(int i, int j, const Vector& k);
    //void setNode(int i, const Dot& u, const Vector& v, const Vector& w, const Vector& k);

    Dot getDot(int i, int j) const;
    Vector getVecDir1(int i, int j) const;
    Vector getVecDir2(int i, int j) const;
    Vector getVecRot(int i, int j) const;

    void setColor(const QColor& color) override;

    qreal getTEnd1() const override;
    qreal getTEnd2() const override;

    SurfaceInterpolated transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;

    SurfaceInterpolated* createCopy() const override;


    SurfaceInterpolated& operator =(const SurfaceInterpolated& obj);

};

class SurfaceKinematic: public Surface{
public:
    Spline* main;
    Spline* dir;

    matrix<4,4> Fpoints;
    Dot (*F)(Dot dot, qreal t1, qreal t2);

    void copy(const SurfaceKinematic& obj);
public:
    SurfaceKinematic();
    SurfaceKinematic(const Spline& main_, const Spline& dir_, Dot (*F_)(Dot, qreal, qreal) = 0, QColor = Qt::black);
    SurfaceKinematic(const SurfaceKinematic&);

    ~SurfaceKinematic();

    void setMainSpline(const Spline& spline);
    void setDirSpline(const Spline& spline);
    void setF(Dot (*F_)(Dot, qreal, qreal));

    void setSubNodesMain(int cnt_);
    void setSubNodesDir(int cnt_);

    Dot getDot(qreal t1, qreal t2) const override;

    void setColor(const QColor& color) override;

    SurfaceKinematic transformed(const matrix<4,4>& F) const;
    void transform(const matrix<4,4>& F) override;

    SurfaceKinematic* createCopy() const override;

    SurfaceKinematic& operator =(const SurfaceKinematic& obj);

};
struct DrawReq{
    matrix<4,4> F;
    qreal zoom;
    Plato plato;
    QWidget* context;
    Vector onViewer;
    Dot viewer;
    QVector<Vector> lights;
    QVector<QPair<Dot, qreal>> lightDots;
    bool useLight;
    Projection projection;
};

#endif // SHAPE_H

#ifndef SHAPE_H
#define SHAPE_H
#include <QColor>
#include <QPainter>
#include <QWidget>
#include <lineutil.h>
#include <vector.h>
class Shape{
protected:
    QColor clr;
    void copy(const Shape* obj);
public:

    Shape(QColor clr = Qt::black);
    Shape(const Shape* obj);
    QColor getColor() const;
    void setColor(const QColor& clr);
    static QPointF transform(DrawReq& drawReq, QPointF point);
    virtual ~Shape();
    virtual void paint(DrawReq& drawReq) = 0;
    virtual Shape* createCopy() = 0;
};

class Line: public Shape{
protected:
    qreal x0,y0,x1,y1;

    void copy(const Line* obj);
public:
    Line(QColor clr = Qt::black);
    Line(qreal x0,qreal y0,qreal x1,qreal y1, QColor clr = Qt::black);
    Line(QPointF p0, QPointF p1, QColor clr = Qt::black);
    Line(const Line* obj);

    QPointF getStart() const;
    QPointF getEnd() const;

    void paint(DrawReq& drawReq) override;
    Line* createCopy() override;

};
class Polygon: public Shape{


    void copy(const Polygon* obj);
    void checkRegularity();

    bool isLeft() const;
public:
    qreal xMn, xMx, yMn, yMx;
    QVector<QPointF> points;
    bool isReagularWindow;
    Polygon(QColor clr = Qt::black);
    Polygon(const QVector<QPointF>& points, QColor clr = Qt::black);
    Polygon(const Polygon* obj);

    void reverse();
    void addPoint(QPointF point);

    void paint(DrawReq& drawReq) override;
    Polygon* createCopy() override;
};


#endif // SHAPE_H

#ifndef SHAPE_H
#define SHAPE_H
#include <QColor>
#include <QPainter>
#include <QWidget>
#include <lineutil.h>

class Shape{
protected:
    QColor clr;
    void copy(const Shape* obj);
    QString name;
public:


    Shape(QColor clr = Qt::black);
    Shape(const Shape* obj);
    QString getName() const;
    QColor getColor() const;
    void setColor(const QColor& clr);
    virtual ~Shape();
    virtual void paint(DrawReq& drawReq) = 0;
    virtual bool build(QPoint pos);
    virtual Shape* createClearObj() = 0;
    virtual Shape* createCopy() = 0;
    virtual QPoint getMinPoint() const = 0;
    virtual QPoint getMaxPoint() const = 0;
    virtual void exportImage(ExpReq& expReq) const = 0;
};
class Dot: public Shape{
    int x,y;

    void copy(const Dot* obj);
public:
    Dot(QColor clr = Qt::black);
    Dot(int x, int y, QColor clr = Qt::black);
    Dot(QPoint pos, QColor clr = Qt::black);
    Dot(const Dot* obj);

    void paint(DrawReq& drawReq) override;
    bool build(QPoint pos) override;
    Dot* createClearObj() override;
    Dot* createCopy() override;
    QPoint getMinPoint() const override;
    QPoint getMaxPoint() const override;
    void exportImage(ExpReq& expReq) const override;
};
class Line: public Shape{
protected:
    QVector<Dot*> dots;
    int x0,y0,x1,y1;
    int step;

    void copy(const Line* obj);
    virtual void buildDots() = 0;
public:
    Line(QColor clr = Qt::black);
    Line(int x0,int y0,int x1,int y1, QColor clr = Qt::black);
    virtual ~Line();

    bool build(QPoint pos) override;
    void paint(DrawReq& drawReq) override;

    QPoint getMinPoint() const override;
    QPoint getMaxPoint() const override;
    void exportImage(ExpReq& expReq) const override;
};
class LineDDA: public Line{
    void copy(const LineDDA* obj);
    void buildDots() override;
public:
    LineDDA(QColor clr = Qt::black);
    LineDDA(int x0,int y0,int x1,int y1, QColor clr = Qt::black);
    LineDDA(const LineDDA* obj);

    LineDDA* createClearObj() override;
    LineDDA* createCopy() override;
};
class LineStep: public Line{
    void copy(const LineStep* obj);
    void buildDots() override;
public:
    LineStep(QColor clr = Qt::black);
    LineStep(int x0,int y0,int x1,int y1, QColor clr = Qt::black);
    LineStep(const LineStep* obj);

    LineStep* createClearObj() override;
    LineStep* createCopy() override;
};
class LineBresenham: public Line{
    QString getCode() const;
    void copy(const LineBresenham* obj);
    void buildDots() override;
public:
    LineBresenham(QColor clr = Qt::black);
    LineBresenham(int x0,int y0,int x1,int y1, QColor clr = Qt::black);
    LineBresenham(const LineBresenham* obj);

    LineBresenham* createClearObj() override;
    LineBresenham* createCopy() override;
};
class LineCastla: public Line{
    QString getCode() const;
    void copy(const LineCastla* obj);
    void buildDots() override;
public:
    LineCastla(QColor clr = Qt::black);
    LineCastla(int x0,int y0,int x1,int y1, QColor clr = Qt::black);
    LineCastla(const LineCastla* obj);

    LineCastla* createClearObj() override;
    LineCastla* createCopy() override;
};

class CircleBresenham: public Shape{
    int r;
    int x0,y0;
    QVector<Dot*> dots;
    int step;

    void copy(const CircleBresenham* obj);
    void buildDots();
public:
    CircleBresenham(QColor clr = Qt::black);
    CircleBresenham(int x0,int y0,int r, QColor clr = Qt::black);
    CircleBresenham(const CircleBresenham* obj);
    ~CircleBresenham();

    bool build(QPoint pos) override;
    void paint(DrawReq& drawReq) override;

    CircleBresenham* createClearObj() override;
    CircleBresenham* createCopy() override;

    QPoint getMinPoint() const override;
    QPoint getMaxPoint() const override;
    void exportImage(ExpReq& expReq) const override;
};
class LineVu: public Line{


    int N =256,M=16;

    void copy(const LineVu* obj);
    void buildDots() override;
    static QPoint turnOct(int x,int y, int oct);
public:
    LineVu(QColor clr = Qt::black);
    LineVu(int x0,int y0,int x1, int y1, QColor clr = Qt::black);
    LineVu(const LineVu* obj);
    LineVu* createClearObj() override;
    LineVu* createCopy() override;
};

#endif // SHAPE_H

#ifndef MYCENTRAL_H
#define MYCENTRAL_H

#include <QWidget>
#include <QPainter>
#include <shape.h>
#include <QMouseEvent>
class MyCentral : public QWidget
{
    Q_OBJECT


public:
    QPoint mousePos;
    Shape* tool;
    Shape* temp;
    QVector<Shape*> shapes;
    bool isBuilding;
    DrawReq drawReq;
    QColor curColor;

    MyCentral(QWidget *parent = nullptr);
    void setTool(Shape *);

    void extracted();
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent*) override;
    void myMouseMoveEvent(QMouseEvent*);
signals:
};

#endif // MYCENTRAL_H

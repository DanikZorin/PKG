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
    Polygon* cutter;
    QVector<Shape*> shapes;
    QVector<Shape*> cutted;

    DrawReq drawReq;

    QColor cutClr = Qt::red;

    MyCentral(QWidget *parent = nullptr);

    void Koen();
    void Barsky();
    void Hodgeman();

    void paintEvent(QPaintEvent *) override;
    void myMouseMoveEvent(QMouseEvent*);

signals:
};

#endif // MYCENTRAL_H

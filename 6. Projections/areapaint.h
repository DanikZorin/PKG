#ifndef AREAPAINT_H
#define AREAPAINT_H

#include <QWidget>
#include "shape.h"
#include "matrix.h"
#include <QVector>
#include <math.h>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QTime>
#include <QDateTime>
double sgn(double a);
class AreaPaint : public QWidget
{
    Q_OBJECT
    QVector <Shape*> shapes;

    qreal angleAlpha, angleBeta, angleGamma;
    int myTimer;
    qreal zoom;
    matrix<3,1> onViewer;
    Plato plato;
    Projection projection;
    Composite* grid;
    qreal gridSize;
    int gridCount;

    DrawReq drawReq;

    long long lastTime;

public:

    AreaPaint(Projection projection_ = Aksonometric, Plato plato_ = Oxy, QWidget *parent = nullptr);

    void updateF();
    void setProjectionType(Projection);
    void setProjectionPlato(Plato);
    void setGridPlato(Plato);

    void keyPressEvent(QKeyEvent*) override;
    void wheelEvent(QWheelEvent*) override;

protected:
    void setFAksonometric();
    void setFAskew();
    void setFCentral();
    void setFPerspective();

    void paintEvent(QPaintEvent*) override;

    void timerEvent(QTimerEvent *) override;
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;

signals:
};

#endif // AREAPAINT_H

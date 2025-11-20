#ifndef DRAWREQ_H
#define DRAWREQ_H

#include <QPainter>
#include <QWidget>
class DrawReq{
public:
    QWidget* context;
    qreal zoom;
    qreal xCntr,yCntr;
    qreal xAskew,yAskew;
    bool isGrid;


    qreal getKoef(){
        return std::min(context->width(),context->height());
    }
};
class ExpReq{
public:

    QImage img;

    QPoint start, end;

};
#endif // DRAWREQ_H

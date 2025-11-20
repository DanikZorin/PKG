#include "lineutil.h"
LineUtil::LineUtil(qreal x0,qreal y0,qreal x1,qreal y1, QColor clr){
    this->x0 = x0;
    this->y0 = y0;
    this->x1 = x1;
    this->y1 = y1;
    this->clr = clr;
}

void LineUtil::paint(DrawReq& drawReq){
    QPainter p(drawReq.context);

    p.setPen(clr);
    QSize screenCntr = drawReq.context->size();
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();

    p.drawLine(screenCntr.width()/2 + (x0+drawReq.xCntr)*scaleKoef + drawReq.xAskew,
               screenCntr.height()/2 + (y0+drawReq.yCntr)*scaleKoef+ drawReq.yAskew,
               screenCntr.width()/2 + (x1+drawReq.xCntr)*scaleKoef+ drawReq.xAskew,
               screenCntr.height()/2 + (y1+drawReq.yCntr)*scaleKoef+ drawReq.yAskew);
}

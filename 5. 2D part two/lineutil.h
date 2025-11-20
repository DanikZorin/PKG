#ifndef LINEUTIL_H
#define LINEUTIL_H

#include <drawreq.h>

class LineUtil{
    qreal x0,y0,x1,y1;
    QColor clr;
public:

    LineUtil(qreal x0,qreal y0,qreal x1,qreal y1, QColor clr = Qt::black);

    void paint(DrawReq& drawReq);
};



#endif // LINEUTIL_H

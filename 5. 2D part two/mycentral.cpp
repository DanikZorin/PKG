#include "mycentral.h"

MyCentral::MyCentral(QWidget *parent): QWidget{parent}{
    setPalette(Qt::white);
    setAutoFillBackground(true);
    setMouseTracking(true);

    cutter = nullptr;

    drawReq.context = this;
    drawReq.xCntr = 0;
    drawReq.yCntr = 0;
    drawReq.zoom = 0.2;
    drawReq.isGrid = true;
}


void MyCentral::Koen(){
    cutted.clear();

    if (!(cutter && cutter->isReagularWindow))
        return;


    int (*code)(QPointF p, Polygon* pol) = [](QPointF p, Polygon* pol){
        int code = 0;
        if (p.y() > pol->yMx)
            code |= 8;
        if (p.y() < pol->yMn)
            code |= 4;
        if (p.x() > pol->xMx)
            code |= 2;
        if (p.x() < pol->xMn)
            code |= 1;

        return code;
    };

    QPointF (*f1)(QPointF a, QPointF b, qreal y0) = [](QPointF a, QPointF b, qreal y0){
        qreal xc = a.x() + (y0-a.y())*(b.x()-a.x())/(b.y()-a.y());
        qreal yc = y0;

        return QPointF(xc, yc);
    };
    QPointF (*f2)(QPointF a, QPointF b, qreal x0) = [](QPointF a, QPointF b, qreal x0){
        qreal xd = x0;
        qreal yd = a.y() + (x0-a.x())*(b.y()-a.y())/(b.x()-a.x());

        return QPointF(xd, yd);
    };

    for (Shape* shape: shapes){
        Line* line;
        if ((line = dynamic_cast<Line*>(shape)) != nullptr){
            int a = code(line->getStart(), cutter);
            int b = code(line->getEnd(), cutter);

            if ((a&b) != 0)
                continue;

            int l = a|b;

            if (l == 0){
                cutted.push_back(new Line(line->getStart(),line->getEnd(), cutClr));
                continue;
            }

            QVector<QPointF> P = {line->getStart(), line->getEnd()};
            if (l&8)
                P.push_back(f1(line->getStart(), line->getEnd(), cutter->yMx));
            if (l&4)
                P.push_back(f1(line->getStart(), line->getEnd(), cutter->yMn));
            if (l&2)
                P.push_back(f2(line->getStart(), line->getEnd(), cutter->xMx));
            if (l&1)
                P.push_back(f2(line->getStart(), line->getEnd(), cutter->xMn));

            QVector<QPointF> res;
            for (QPointF point: P){
                int c = code(point, cutter);
                if (c == 0){
                    bool isUnique = true;
                    for (QPointF pRes: res)
                        if (pRes == point)
                            isUnique = false;

                    if (isUnique)
                        res.push_back(point);
                }
            }
            if (res.size()==2){
                cutted.push_back(new Line(res[0],res[1],cutClr));
            }
            else if (res.size() >2){
                qDebug()<<res.size()<<" Koen unreadable!";
            }
        }
    }


    update();
}
void MyCentral::Barsky(){
    cutted.clear();

    if (!(cutter && cutter->isReagularWindow))
        return;

    for (Shape* shape: shapes){
        Line* line;
        if ((line = dynamic_cast<Line*>(shape)) != nullptr){
            QPointF a = line->getStart();
            QPointF b = line->getEnd();

            qreal p[] = {a.x()-b.x(), b.x()-a.x(), a.y()-b.y(), b.y()-a.y()};
            qreal q[] = {a.x()-cutter->xMn, cutter->xMx-a.x(), a.y()-cutter->yMn, cutter->yMx-a.y()};

            qreal tStart = 0;
            qreal tEnd = 1;


            bool isOk = true;
            for (int i=0;i<4;i++){
                if (p[i] == 0)
                    if (q[i] < 0){
                        isOk = false;
                        break;
                    }
                if (p[i] > 0)
                    tEnd = qMin(q[i]/p[i], tEnd);
                if (p[i] < 0)
                    tStart = qMax(q[i]/p[i], tStart);

                if (tStart >= tEnd){
                    isOk = false;
                    break;
                }
            }

            if (isOk)
                cutted.push_back(new Line(a+tStart*(b-a),a+tEnd*(b-a),cutClr));
        }

    }



    update();
}
QPointF getS(QPointF A0,QPointF A1,QPointF C0,QPointF C1){
    /*
    x = A0x + t1*(A1x-A0x) = C0x + t2*(C1x-C0x)
    y = A0y + t1*(A1y-A0y) = C0y + t2*(C1y-C0y)


    t1 = ( C0x-A0x + t2*(C1x-C0x) ) / (A1x-A0x)

    ( (C0x-A0x)*(A1y-A0y)-(C0y-A0y)*(A1x-A0x) ) / ( (C1y-C0y)*(A1x-A0x) - (C1x-C0x)*(A1y-A0y) ) =  t2

     t1 = (C0-A0)x/(A1-A0)x + t2*(C1x-C0x)/(A1-A0)x
    ( A0y-C0y + (C0-A0)x/(A1-A0)x(A1y-A0y) )/ ( (C1y-C0y) - (C1x-C0x)(A1y-A0y)/(A1-A0)x ) =  t2
    */
    qreal t2 = ( (C0.x()-A0.x())*(A1.y()-A0.y())-(C0.y()-A0.y())*(A1.x()-A0.x()) ) / ( (C1.y()-C0.y())*(A1.x()-A0.x()) - (C1.x()-C0.x())*(A1.y()-A0.y()) );
    QPointF s = C0 + t2*(C1-C0);
    return s;
}
void MyCentral::Hodgeman(){
    cutted.clear();
    if (!cutter)
        return;

    QVector<QPointF> c = cutter->points;
    c.push_back(cutter->points[0]);

    for (Shape* shape: shapes){
        Polygon* polygon;
        if ((polygon = dynamic_cast<Polygon*>(shape))!=nullptr){

            QVector<QPointF> a = polygon->points;
            for (int k=0;k<cutter->points.size();k++){
                QVector<QPointF> a_;

                Vector v1(c[k],c[k+1]);
                Vector v2(c[k],a[0]);
                bool V = (vectorMult(v1,v2)>=0);
                for (int i=0;i<a.size();i++){
                    Vector v3(c[k],a[(i+1)%a.size()]);
                    bool V_ = (vectorMult(v1,v3)>=0);

                    if (V && V_)
                        a_.push_back(a[(i+1)%a.size()]);
                    if (V && !V_)
                        a_.push_back(getS(a[i],a[(i+1)%a.size()],c[k],c[k+1]));
                    if (!V && V_){
                        a_.push_back(getS(a[i],a[(i+1)%a.size()],c[k],c[k+1]));
                        a_.push_back(a[(i+1)%a.size()]);
                    }
                    V = V_;
                }
                a = a_;
                if (a_.size() == 0){
                    break;
                }
            }
            cutted.push_back(new Polygon(a, cutClr));
        }
    }


    update();
}

void MyCentral::Nickolla(){
    cutted.clear();
    if (!(cutter && cutter->isReagularWindow))
        return;



    void (*fMinusPiHalf)(QPointF& p) = [](QPointF& p){
        qreal x = p.x();
        qreal y = p.y();
        p.setX(y);
        p.setY(-x);
    };
    void (*fPi)(QPointF& p) = [](QPointF& p){
        qreal x = p.x();
        qreal y = p.y();
        p.setX(-x);
        p.setY(-y);
    };
    void (*fPiHalf)(QPointF& p) = [](QPointF& p){
        qreal x = p.x();
        qreal y = p.y();
        p.setX(-y);
        p.setY(x);
    };
    void (*fxy)(QPointF& p) = [](QPointF& p){
        qreal x = p.x();
        qreal y = p.y();
        p.setX(-y);
        p.setY(-x);
    };
    void (*fx)(QPointF& p) = [](QPointF& p){
        qreal x = p.x();
        qreal y = p.y();
        p.setX(x);
        p.setY(-y);
    };


    qreal xMn = cutter->xMn;
    qreal yMn = cutter->yMn;
    qreal xMx = cutter->xMx;
    qreal yMx = cutter->yMx;


    QPointF C1;
    QPointF C2;
    QPointF C3;
    QPointF C4;
    QPointF A;
    QPointF B;

    QPointF Mn = {xMn,yMn};
    QPointF Mx = {xMx,yMx};

    std::stack<void(*)(QPointF&)> actions;


    auto cutHor = [&](qreal y0){
        QPointF C;

        C.setX(A.x() + (y0-A.y())*(B.x()-A.x())/(B.y()-A.y()));
        C.setY(y0);

        return C;
    };

    auto cutVer = [&](qreal x0){
        QPointF C;

        C.setX(x0);
        C.setY(A.y() + (x0-A.x())*(B.y()-A.y())/(B.x()-A.x()));

        return C;
    };

    auto rev = [&](void(*f)(QPointF&)){
        if (f==fMinusPiHalf)
            return fPiHalf;
        if (f==fPiHalf)
            return fMinusPiHalf;
        return f;
    };

    auto t = [&](void(*f)(QPointF&)){

        f(Mn);
        f(Mx);

        if (Mn.x() > Mx.x()){
            int x = Mn.x();
            Mn.setX(Mx.x());
            Mx.setX(x);
        }
        if (Mn.y() > Mx.y()){
            int y = Mn.y();
            Mn.setY(Mx.y());
            Mx.setY(y);
        }

        C1 = {Mn.x(),Mx.y()};
        C2 = {Mx.x(),Mx.y()};
        C3 = {Mn.x(),Mn.y()};
        C4 = {Mx.x(),Mn.y()};

        f(A);
        f(B);

        actions.push(rev(f));
    };


    for (Shape* shape: shapes){
        Line* line;
        if ((line=dynamic_cast<Line*>(shape)) != nullptr){

            while(actions.size()){
                actions.pop();
            }
            C1 = {xMn, yMx};
            C2 = {xMx, yMx};
            C3 = {xMn, yMn};
            C4 = {xMx, yMn};

            A = line->getStart();
            B = line->getEnd();

            if (A.x() > Mx.x()){
                t(fPi);
            }
            if (A.x() < Mn.x()){
                if (B.x() < Mn.x()){
                    continue;
                }
                if (A.y() < Mn.y()){
                    t(fx);
                }
            }
            if (Mx.x() >= A.x() && A.x() >= Mn.x()){
                if (A.y() > Mx.y()){
                    t(fPiHalf);
                }
                else if (A.y() < Mn.y()){
                    t(fMinusPiHalf);

                    if (B.x() < Mn.x()){
                        continue;
                    }
                }
            }

            if (A.x() < Mn.x() && A.y() > Mx.y() && B.x() >= Mn.x()){
                // CASE 1

                if (B.y() > Mx.y()){
                    continue;
                }

                Vector AC1(A,C1);
                Vector AB(A,B);

                if (vectorMult(AC1,AB) > 0){
                    t(fxy);
                }

                if (B.y() >= Mn.y()){
                    A = cutVer(Mn.x());
                    if (B.x() > Mx.x()){
                        B = cutVer(Mx.x());
                    }
                }
                else{ // B.y() < Mn.y()
                    Vector AC3(A,C3);
                    if (vectorMult(AC3,AB)<=0){
                        continue;
                    }
                    if (B.x() > Mx.x()){
                        Vector AC4(A,C4);
                        if (vectorMult(AC4,AB)<=0){
                            B = cutHor(Mn.y());
                        }
                        else{
                            B = cutVer(Mx.x());
                        }
                    }
                    else{
                        B = cutHor(Mn.y());
                    }
                    A = cutVer(Mn.x());
                }
            }
            else if (A.x() < Mn.x() && Mx.y() >= A.y() && A.y() >= Mn.y() && B.x() >= Mn.x()){
                // CASE 2
                if (Mx.y() >= B.y() && B.y() >= Mn.y()){
                    A = cutVer(Mn.x());
                    if (B.x() > Mx.x()){
                        B = cutVer(Mx.x());
                    }
                }
                else{
                    if (B.y() > Mx.y()){
                        t(fx);
                    }
                    Vector AB(A,B);
                    Vector AC3(A,C3);
                    if (vectorMult(AC3,AB)<=0){
                        continue;
                    }

                    if (B.x() <= Mx.x()){
                        B = cutHor(Mn.y());
                    }
                    else{
                        Vector AC4(A,C4);
                        if (vectorMult(AC4,AB)<=0){
                            B = cutHor(Mn.y());
                        }
                        else{
                            B = cutVer(Mx.x());
                        }
                    }
                    A = cutVer(Mn.x());
                }
            }
            else if (Mx.x() >= A.x() && A.x() >= Mn.x() && Mx.y() >= A.y() && A.y() >= Mn.y()){
                // CASE 3

                if (B.x() < Mn.x() && B.y() > Mx.y()){
                    // SUB-CASE 1

                    Vector AB(A,B);
                    Vector AC1(A,C1);

                    if (vectorMult(AC1,AB) < 0){
                        B = cutHor(Mx.y());
                    }
                    else{
                        B = cutVer(Mn.x());
                    }
                }
                else if (B.x() < Mn.x() && Mx.y() >= B.y() && B.y() >= Mn.y()){
                    B = cutVer(Mn.x());
                }
            }

            while(actions.size()){
                auto fRev = actions.top();

                fRev(A);
                fRev(B);

                actions.pop();
            }

            cutted.push_back(new Line(A,B, Qt::red));
        }
    }


    update();
}




void MyCentral::paintEvent(QPaintEvent*){
    QPainter p;

    QSize screenCntr = drawReq.context->size();
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();

    qreal xL = (-screenCntr.width()/2.-drawReq.xAskew)/scaleKoef-drawReq.xCntr -1;
    qreal yU = (-screenCntr.height()/2.-drawReq.yAskew)/scaleKoef-drawReq.yCntr -1;
    qreal xR =  (screenCntr.width()/2.-drawReq.xAskew)/scaleKoef-drawReq.xCntr +1;
    qreal yD =  (screenCntr.height()/2.-drawReq.yAskew)/scaleKoef-drawReq.yCntr +1;

    if (drawReq.isGrid){
        int lineXCnt = xR-xL;
        int lineYCnt = yD-yU;

        int dx = 1;
        int dy = 1;
        int mxCnt = 25;
        while (lineXCnt >= mxCnt && lineYCnt >= mxCnt){
            lineXCnt /= 5;
            lineYCnt /= 5;
            dx *= 5;
            dy *= 5;
        }
        for (qreal xi = dx*int(xL/dx); xi<=xR;xi+=dx){
            LineUtil line(xi,yU,xi,yD,Qt::gray);
            line.paint(drawReq);
            p.begin(this);

            p.drawText(screenCntr.width()/2 + (xi+drawReq.xCntr)*scaleKoef + drawReq.xAskew-10,
                       screenCntr.height()/2 + (0+drawReq.yCntr)*scaleKoef+ drawReq.yAskew+10,QString::number(xi));
            p.end();
        }
        for (qreal yi = dy*int(yU/dy); yi<=yD;yi+=dy){
            LineUtil line(xL,yi,xR,yi,Qt::gray);
            line.paint(drawReq);
            p.begin(this);

            if (-yi != 0){
                p.drawText(screenCntr.width()/2 + (0+drawReq.xCntr)*scaleKoef + drawReq.xAskew,
                           screenCntr.height()/2 + (yi+drawReq.yCntr)*scaleKoef+ drawReq.yAskew+10,QString::number(-yi));
            }
            p.end();
        }
    }
    for (Shape* shape: shapes){
        shape->paint(drawReq);
    }
    if (cutter){
        cutter->paint(drawReq);
    }
    for (Shape* shape: cutted){
        shape->paint(drawReq);
    }



    if (drawReq.isGrid){
        LineUtil lineX(xL, 0, xR, 0, Qt::red);
        lineX.paint(drawReq);
        LineUtil lineY(0, yU, 0, yD, Qt::green);
        lineY.paint(drawReq);
    }
}
void MyCentral::myMouseMoveEvent(QMouseEvent* e){
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();


    qreal x_ = (e->pos().x()-pos().x()-width()/2.-drawReq.xAskew)/scaleKoef-drawReq.xCntr;
    int x = x_ - (x_<0?1:0);
    qreal y_ = (e->pos().y()-pos().y()-height()/2.-drawReq.yAskew)/scaleKoef-drawReq.yCntr+1; /// don't forget 'bout minus
    int y = -y_ + (y_<0?1:0);

    mousePos = {x, y};

}


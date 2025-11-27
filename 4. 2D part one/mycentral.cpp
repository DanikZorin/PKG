#include "mycentral.h"

MyCentral::MyCentral(QWidget *parent): QWidget{parent}{
    setPalette(Qt::white);
    setAutoFillBackground(true);
    setMouseTracking(true);


//    shapes.push_back(new Dot(0,0));
//    shapes.push_back(new Dot(-1,-1));
//    shapes.push_back(new Dot(-3,0));
//    shapes.push_back(new LineDDA(-3,-3, 10,4));


    temp = nullptr;
    isBuilding = false;
    tool = nullptr;
    curColor = Qt::black;

    drawReq.context = this;
    drawReq.xCntr = 0;
    drawReq.yCntr = 0;
    drawReq.zoom = 0.2;
    drawReq.isAskew = true;
    drawReq.isGrid = true;
}
void MyCentral::setTool(Shape* tool){
    if (this->tool)
        delete this->tool;
    this->tool = tool;
    if (this->tool)
        this->tool->setColor(curColor);
}

void MyCentral::paintEvent(QPaintEvent*){
    QPainter p;

    QSize screenCntr = drawReq.context->size();
    qreal scaleKoef = drawReq.zoom*drawReq.getKoef();
    bool ask = drawReq.isAskew;

    qreal xL = (-screenCntr.width()/2.-drawReq.xAskew)/scaleKoef+.5*ask-drawReq.xCntr -1;
    qreal yU = (-screenCntr.height()/2.-drawReq.yAskew)/scaleKoef+.5*ask-drawReq.yCntr -1;
    qreal xR =  (screenCntr.width()/2.-drawReq.xAskew)/scaleKoef+.5*ask-drawReq.xCntr +1;
    qreal yD =  (screenCntr.height()/2.-drawReq.yAskew)/scaleKoef+.5*ask-drawReq.yCntr +1;

    if (drawReq.isGrid){
        int lineXCnt = xR-xL;
        int lineYCnt = yD-yU;

        int dx = 1;
        int dy = 1;
        int mxCnt = 15;
        int step = 5;
        while (lineXCnt >= mxCnt && lineYCnt >= mxCnt){
            lineXCnt /= step;
            lineYCnt /= step;
            dx *= step;
            dy *= step;
        }
        qreal koef = (qreal)qMin(lineXCnt, lineYCnt) / mxCnt; /// (0;1)
        if (dx != 1){
            for (qreal xi = dx*int(xL/dx)-.5*ask; xi<=xR;xi+=dx){
                for (int i=-step;i<=step;i++){
                    QColor clr(255*koef,255*koef,255*koef);
                    LineUtil line(xi+(qreal)i*dx/step,yU,xi+(qreal)i*dx/step,yD,clr);
                    line.paint(drawReq);
                }
            }
            for (qreal yi = dy*int(yU/dy)+.5*ask; yi<=yD;yi+=dy){
                for (int i=-step;i<=step;i++){
                    QColor clr(255*koef,255*koef,255*koef);
                    LineUtil line(xL,yi+(qreal)i*dy/step,xR,yi+(qreal)i*dy/step,clr);
                    line.paint(drawReq);
                }
            }
        }
        for (qreal xi = dx*int(xL/dx)-.5*ask; xi<=xR;xi+=dx){
            LineUtil line(xi,yU,xi,yD);
            line.paint(drawReq);

            p.begin(this);

            p.drawText(screenCntr.width()/2 + (xi+0.5*ask+drawReq.xCntr)*scaleKoef + drawReq.xAskew-10,
                       screenCntr.height()/2 + (0+drawReq.yCntr)*scaleKoef+ drawReq.yAskew+10,QString::number(xi+0.5*ask));
            p.end();
        }
        for (qreal yi = dy*int(yU/dy)+.5*ask; yi<=yD;yi+=dy){

            LineUtil line(xL,yi,xR,yi);
            line.paint(drawReq);
            p.begin(this);

            if (-yi+0.5*ask != 0){
                p.drawText(screenCntr.width()/2 + (0+drawReq.xCntr)*scaleKoef + drawReq.xAskew,
                           screenCntr.height()/2 + (yi-0.5*ask+drawReq.yCntr)*scaleKoef+ drawReq.yAskew+10,QString::number(-yi+0.5*ask));
            }
            p.end();
        }
    }
    for (Shape* shape: shapes){
        shape->paint(drawReq);
    }
    if (temp){
        temp->paint(drawReq);
        delete temp;
        temp = nullptr;
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
    bool ask = drawReq.isAskew;


    qreal x_ = (e->pos().x()-pos().x()-width()/2.-drawReq.xAskew)/scaleKoef+.5*ask-drawReq.xCntr;
    int x = x_ - (x_<0?1:0);
    qreal y_ = (e->pos().y()-pos().y()-height()/2.-drawReq.yAskew)/scaleKoef-0.5*ask-drawReq.yCntr+1; /// don't forget 'bout minus
    int y = -y_ + (y_<0?1:0);

    mousePos = {x, y};

    if (tool){
        temp = tool->createCopy();
        temp->build(mousePos);
        update();
    }
}
void MyCentral::mousePressEvent(QMouseEvent* e){

    if (tool){
        if (e->buttons() == Qt::RightButton){
            Shape* toDel = tool;
            tool = tool->createClearObj();
            tool->setColor(curColor);
            delete toDel;
        }
        else{
            isBuilding = !tool->build(mousePos);
            if (!isBuilding){
                shapes.push_back(tool);
                tool = tool->createClearObj();
                tool->setColor(curColor);
            }
        }
        update();
    }
}

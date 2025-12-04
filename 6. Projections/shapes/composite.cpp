#include "shape.h"

Composite::Composite(): Shape(){}
Composite::Composite(const QVector<Shape*>& shapes_): Composite(){    
    for (Shape* shape: shapes_){
        addShape(shape);
    }
}
Composite::Composite(const Composite& obj){
    (*this) = obj;
}

Composite::~Composite(){
    clearShapes();
}
void Composite::update(){
    extractedPolygons.clear();

    for (Shape* shape: shapes){
        QVector<Polygon> p_ = shape->extractPolygons();
        for (Polygon& polygon: p_){
            extractedPolygons.push_back(polygon);
        }
    }

}
void Composite::clearShapes(){
    for (Shape* shape: shapes){
        delete shape;
    }
    shapes.clear();
    extractedPolygons.clear();
}
void Composite::copy(const Composite& obj){
    Shape::copy(obj);
    for (Shape* shape: obj.getShapes()){
        addShape(shape);
    }
}

QVector<Shape*> Composite::getShapes() const{
    return shapes;
}

void Composite::addShape(const Shape*const shape_){
    Shape* shape = shape_->createCopy();
    shapes.push_back(shape);
    QVector<Polygon> p_ = shape->extractPolygons();
    for (Polygon& polygon: p_){
        extractedPolygons.push_back(polygon);
    }
}
void Composite::addShapeDel(Shape* shape_){
    addShape(shape_);
    delete shape_;
}
QVector<qreal> Composite::getPolygonsDepth(const DrawReq& drawReq) const{
    QVector<qreal> depths;
    for (const Polygon& polygon: extractedPolygons){
        depths.push_back(polygon.getDepth(drawReq));
    }
    return depths;
}
QVector<Dot> Composite::extractDots() const{
    QVector<Dot> dots;
    for (Shape* shape: shapes){
        if (typeid(*shape) == typeid(Dot)){
            Dot* dot = dynamic_cast<Dot*>(shape);
            dots.push_back(*dot);
        }
        if (typeid(*shape) == typeid(Line)){
            Line* line = dynamic_cast<Line*>(shape);
            dots.push_back(line->getBegin());
            dots.push_back(line->getEnd());
        }
        if (typeid(*shape) == typeid(Polygon)){
            Polygon* polygon = dynamic_cast<Polygon*>(shape);
            QVector<Dot> curDots = polygon->getDots();
            for (const Dot& dot: curDots){
                dots.push_back(dot);
            }
        }
        if (typeid(*shape) == typeid(Polyedr)){
            Polyedr* polyedr = dynamic_cast<Polyedr*>(shape);
            QVector<Polygon> curPolygons = polyedr->getPolygons();
            for (const Polygon& polygon: curPolygons){
                QVector<Dot> curDots = polygon.getDots();
                for (const Dot& dot: curDots){
                    dots.push_back(dot);
                }
            }
        }
        if (typeid(*shape) == typeid(Composite)){
            Composite* composite = dynamic_cast<Composite*>(shape);
            QVector <Dot> curDots = composite->extractDots();
            for (const Dot& dot: curDots){
                dots.push_back(dot);
            }
        }
    }
    return dots;
}
QVector<Dot> Composite::getConvexBoundDots() const{ // Jarvies' algorithm by Ozx projection
    QVector <Dot> dots = extractDots();
    if (dots.size() == 0){
        return QVector<Dot>();
    }
    QVector <Dot> res;
    QVector <Dot> temp;
    Dot startDot = dots[0];

    for (const Dot& dot: dots){
        if (dot.getX() < startDot.getX()){
            startDot = dot;
        }
        else if (dot.getX() == startDot.getX() && dot.getX() < startDot.getZ()){
            startDot = dot;
        }
    }

    //    qDebug()<<"startDot:"<<startDot.getY();
    res.push_back(startDot);

    for (const Dot& dot: dots){
        if (!(dot == startDot)){
            temp.push_back(dot);
        }
    }

    while (res.size() < dots.size()){
        if (res.size() == 3){
            temp.push_back(startDot);
        }

        Dot lastDot = res[res.size()-1];
        //        qDebug()<<"lastDot:"<<lastDot.getY();
        Dot nextDot = temp[0];
        Vector nextVec = nextDot - lastDot;
        int nextI = 0;

        for (int i=1;i<temp.size();i++){
            Dot newDot = temp[i];
            Vector newVec = newDot - lastDot;

            qreal val = (nextVec^newVec).getY();
            //            qDebug()<<"nextDot:"<<nextDot.getY()<<"newDot:"<<newDot.getY();
            //            qDebug()<<"nextVec:"<<nextVec.getX()<<nextVec.getY()<<nextVec.getZ();
            //            qDebug()<<"newVec:"<<newVec.getX()<<newVec.getY()<<newVec.getZ();
            //            qDebug()<<"val:"<<val;
            //            qDebug();

            if (val == 0){
                if (newVec.getModule() < nextVec.getModule()){
                    temp.remove(i);
                }
                else if (newVec.getModule() > nextVec.getModule()){
                    temp.remove(nextI);

                    nextI = i;
                    nextVec = newVec;
                    nextDot = newDot;
                }
                i--;
                continue;
            }
            else if (val < 0){
                nextI = i;
                nextVec = newVec;
                nextDot = newDot;
            }
        }

        if (nextDot == startDot){
            //            qDebug()<<"OVER";
            break;
        }
        else{
            temp.remove(nextI);
            //            qDebug()<<"Dot"<<nextDot->getY()<<"has been added. temp.size():"<<temp.size()<<"\n";
            res.push_back(nextDot);
        }
    }
    return res;
}
Polygon* Composite::getConvexBound() const{
    return new Polygon(getConvexBoundDots(), Qt::black);
}
Composite* Composite::getTriangulationDelone() const{
    QVector<Dot> dots = extractDots();

    Composite* res = new Composite();
    if (dots.size() < 3){
        return res;
    }

    struct DotData{
        Dot dot;

        DotData* next;
    };
    QVector<DotData*> data = {new DotData(),new DotData(),new DotData()};
    QVector<Polygon*> triangles;

    Vector a01 = dots[1] - dots[0];
    Vector a12 = dots[2] - dots[1];

    if ((a01^a12).getY() > 0){
        data[0]->dot = dots[0];
        data[1]->dot = dots[1];
        data[2]->dot = dots[2];
    }
    else if ((a01^a12).getY() < 0) {
        data[0]->dot = dots[0];
        data[1]->dot = dots[2];
        data[2]->dot = dots[1];
    }
    else{
        qDebug()<<"straight triangle!";
        throw 1;
    }
    data[0]->next = data[1];
    data[1]->next = data[2];
    data[2]->next = data[0];


    triangles.push_back(new Polygon({data[0]->dot, data[1]->dot, data[2]->dot}, Qt::black));
    DotData* startData = data[0];

    QVector<QPair<Dot,Dot>> visibleVerbs;

    for (int i=3;i<dots.size();i++){

        DotData* newData = new DotData();
        data.push_back(newData);
        newData->dot = dots[i];

        bool insideTriangle = false;
        bool onEdge = false;

        // belong check
        for (int iTr = 0, cntTr = triangles.size(); iTr < cntTr; iTr++){
            Polygon* triangle = triangles[iTr];
            QVector<Dot> dotsTriangle = triangle->getDots();

            if (dotsTriangle.size() != 3){
                qDebug()<<"not a triangle!";
                throw 1;
            }

            Vector a = dotsTriangle[1] - dotsTriangle[0];
            Vector b = dotsTriangle[2] - dotsTriangle[0];
            Vector toNew = newData->dot - dotsTriangle[0];

            matrix<2,3> belongMat({{a.getZ(), b.getZ(), toNew.getZ()}
                                  ,{a.getX(), b.getX(), toNew.getX()}});

            belongMat.makeEqualE();

            qreal k1 = belongMat[0][2];
            qreal k2 = belongMat[1][2];

            /*qDebug()<<"dotsTriangle:"<<dotsTriangle.size();
            for (const Dot& dot: dotsTriangle){
                dot.show();
            }
            newData->dot.show();
            qDebug()<<"k:"<<k1<<" "<<k2;
*/
            if (k1 > 0 && k2 > 0 && k1 + k2 < 1){

                insideTriangle = true;

                delete triangles[iTr];
                triangles.remove(iTr--);
                cntTr--;

                visibleVerbs.push_back({dotsTriangle[0], dotsTriangle[1]});
                visibleVerbs.push_back({dotsTriangle[0], dotsTriangle[2]});
                visibleVerbs.push_back({dotsTriangle[1], dotsTriangle[2]});

                //qDebug()<<"Dot in center. new Triangles:";
                Polygon* tri;
                tri = new Polygon({dotsTriangle[0], dotsTriangle[1], newData->dot}, Qt::black);
                /*for (const Dot& dot: tri->getDots()){
                    dot.show();
                }*/
                //qDebug()<<"Must be:";
                //dotsTriangle[0].show();
                //dotsTriangle[1].show();
                //newData->dot.show();
                triangles.push_back(tri);
                tri = new Polygon({dotsTriangle[0], dotsTriangle[2], newData->dot}, Qt::black);
                /*for (const Dot& dot: tri->getDots()){
                    dot.show();
                }*/
                //qDebug()<<"Must be:";
                //dotsTriangle[0].show();
                //dotsTriangle[2].show();
                //newData->dot.show();
                triangles.push_back(tri);
                tri = new Polygon({dotsTriangle[1], dotsTriangle[2], newData->dot}, Qt::black);
                /*for (const Dot& dot: tri->getDots()){
                    dot.show();
                }*/
                //qDebug()<<"Must be:";
                //dotsTriangle[1].show();
                //dotsTriangle[2].show();
                //newData->dot.show();
                triangles.push_back(tri);


                break;
            }
            else if ((k1 == 0 && k2 < 1) || (k2 == 0 && k1 < 1) || k1 + k2 == 1){

                onEdge = true;

                delete triangles[iTr];
                triangles.remove(iTr--);
                cntTr--;

                if (k1 == 0){
                    visibleVerbs.push_back({dotsTriangle[0], dotsTriangle[1]});
                    visibleVerbs.push_back({dotsTriangle[1], dotsTriangle[2]});

                    triangles.push_back(new Polygon({dotsTriangle[0],dotsTriangle[1], newData->dot}, Qt::black));
                    triangles.push_back(new Polygon({dotsTriangle[1],dotsTriangle[2], newData->dot}, Qt::black));
                }
                else if (k2 == 0){
                    visibleVerbs.push_back({dotsTriangle[0], dotsTriangle[2]});
                    visibleVerbs.push_back({dotsTriangle[1], dotsTriangle[2]});

                    triangles.push_back(new Polygon({dotsTriangle[0], dotsTriangle[2], newData->dot}, Qt::black));
                    triangles.push_back(new Polygon({dotsTriangle[1], dotsTriangle[2], newData->dot}, Qt::black));
                }
                else if (k1 + k2 == 1){
                    visibleVerbs.push_back({dotsTriangle[0],  dotsTriangle[1]});
                    visibleVerbs.push_back({dotsTriangle[0],  dotsTriangle[2]});

                    triangles.push_back(new Polygon({dotsTriangle[0], dotsTriangle[1], newData->dot}, Qt::black));
                    triangles.push_back(new Polygon({dotsTriangle[0], dotsTriangle[2], newData->dot}, Qt::black));
                }
            }
        }

        if (!onEdge && !insideTriangle){

            DotData* L = startData;
            DotData* R = startData;

            for (DotData* curData = startData->next; curData != startData; curData = curData->next){
                Vector toCur = curData->dot - newData->dot;

                Vector toL = L->dot - newData->dot;

                if ((toL^toCur).getY() > 0){
                    L = curData;
                }

                Vector toR = R->dot - newData->dot;
                if ((toR^toCur).getY() < 0){
                    R = curData;
                }
            }

            for (DotData* curData = L; curData != R; curData = curData->next){
                visibleVerbs.push_back({curData->dot, curData->next->dot});
                triangles.push_back(new Polygon({curData->dot, curData->next->dot, newData->dot}, Qt::black));
            }

            L->next = newData;
            newData->next = R;
        }

        for (int j=0;j<visibleVerbs.size();j++){

            bool isOnBound = false;

            Dot A1 = visibleVerbs[j].first;
            Dot A2 = visibleVerbs[j].second;

            DotData* curData = startData;
            do{
                if ((A1 == curData->dot && A2 == curData->next->dot) || (A2 == curData->dot && A1 == curData->next->dot)){
                    isOnBound = true;
                    break;
                }
                curData = curData->next;

            } while(curData != startData);

            if (isOnBound){
                visibleVerbs.remove(j--);
                continue;
            }

            Dot B1;
            Dot B2;

            int tr1Num = -1, tr2Num = -1;

            //qDebug()<<"triangles.size:"<<triangles.size();

            for (int iTr = 0;iTr<triangles.size();iTr++){

                Polygon* triangle = triangles[iTr];
                QVector<Dot> dotsTriangle = triangle->getDots();

                //qDebug()<<dotsTriangle[0].getName()<<dotsTriangle[1].getName()<<dotsTriangle[2].getName();
                bool A1Found = false, A2Found = false;

                int A1Pos = -1, A2Pos = -1;
                for (int k = 0;k<dotsTriangle.size();k++){
                    if (dotsTriangle[k] == A1){
                        A1Found = true;
                        A1Pos = k;
                        continue;
                    }
                    if (dotsTriangle[k] == A2){
                        A2Found = true;
                        A2Pos = k;
                        continue;
                    }
                }

                if (A1Found && A2Found){
                    if (tr1Num == -1){
                        tr1Num = iTr;
                        B1 = dotsTriangle[3-A1Pos-A2Pos];
                    }
                    else if (tr2Num == -1){
                        tr2Num = iTr;
                        B2 = dotsTriangle[3-A1Pos-A2Pos];
                    }
                    else{
                        qDebug()<<"too many triangles!(";
                    }
                }
            }

            Vector B1A1 = A1 - B1;
            Vector B1A2 = A2 - B1;
            Vector B2A1 = A1 - B2;
            Vector B2A2 = A2 - B2;

//            qDebug()<<"A1:"<<A1.getX()<<A1.getY()<<A1.getZ();
 //           qDebug()<<"A2:"<<A2.getX()<<A2.getY()<<A2.getZ();
  //          qDebug()<<"B1:"<<B1.getX()<<B1.getY()<<B1.getZ();
   //         qDebug()<<"B2:"<<B2.getX()<<B2.getY()<<B2.getZ();
            qreal DeloneVal = qAbs((B1A1^B1A2).getY())*(B2A1*B2A2)+qAbs((B2A1^B2A2).getY())*(B1A1*B1A2);

    //        qDebug()<<"DeloneVal:"<<DeloneVal;
            if (DeloneVal < 0){

                delete triangles[tr1Num];
                triangles.remove(tr1Num);

                delete triangles[tr2Num-(tr1Num<tr2Num)];
                triangles.remove(tr2Num-(tr1Num<tr2Num));

                triangles.push_back(new Polygon({A1, B1, B2}, Qt::black));
                triangles.push_back(new Polygon({A2, B1, B2}, Qt::black));

                Dot B;
                if (B1 == newData->dot){
                    B = B2;
                }
                else if (B2 == newData->dot){
                    B = B1;
                }
                else{
                    qDebug()<<"bad triangles!";
                    throw 1;
                }

                visibleVerbs.push_back({A1,B});
                visibleVerbs.push_back({A2,B});
            }
      //      qDebug()<<"######### Edge end  ###############";
            visibleVerbs.remove(j--);
        }
      //  qDebug()<<"######### Dot end  ###############";
    }

    for (Polygon* triangle: triangles){
        res->addShape(triangle);
    }
    for (DotData* dotData: data){
        delete dotData;
    }
    return res;
}
Dot Composite::getCenter() const{
    if (isCenterSet){
        return Dot(mCenter[0][0],mCenter[1][0],mCenter[2][0]);
    }
    Dot center(0,0,0);
    for (Shape* shape: shapes){
        center+=shape->getCenter();
    }
    return center/shapes.size();
}
QVector<Polygon> Composite::extractPolygons() const{
    return extractedPolygons;
}
void Composite::setColor(const QColor& color){
    for (Shape* shape: shapes){
        shape->setColor(color);
    }
    update();
}

Composite Composite::transformed(const matrix<4,4>& F) const{
    Composite res(*this);
    res.transform(F);
    return res;
}
void Composite::transform(const matrix<4,4>& F){
    Shape::transform(F);
    for (Shape* shape: shapes){
        shape->transform(F);
    }
}

void Composite::drawByDrawman(const DrawReq& drawReq) const{
    QVector<qreal> depths = getPolygonsDepth(drawReq);
    QVector<Polygon> polygons(extractedPolygons);
    QVector<Polygon*> polygonsP;
    for (Polygon& p: polygons){
        polygonsP.push_back(&p);
    }
    for (int i=0;i<polygons.size()-1;i++){
        for(int j=i+1;j<polygons.size();j++){
            if (depths[i] < depths[j]){
                std::swap(depths[i], depths[j]);
                std::swap(polygonsP[i], polygonsP[j]);
            }
        }
    }
//    int i=0;
    for (const Polygon* polygon: polygonsP){
        //qDebug()<<i++;
        polygon->draw(drawReq);
    }
}
void Composite::draw(const DrawReq& drawReq) const{
    for (Shape* shape: shapes){
        if (!dynamic_cast<Polygon*>(shape) && !dynamic_cast<Polyedr*>(shape) && !dynamic_cast<Surface*>(shape)){
            shape->draw(drawReq);
        }
    }
    drawByDrawman(drawReq);
}
Composite* Composite::createCopy() const{
    return new Composite(*this);
}

Composite& Composite::operator=(const Composite& obj){
    if (&obj != this){
        clearShapes();
        copy(obj);
    }
    return *this;
}

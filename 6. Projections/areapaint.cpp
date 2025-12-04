#include "areapaint.h"
double sgn(double a){
    if (a==0)
        return 0;
    else
        return std::abs(a)/a;
}
AreaPaint::AreaPaint(Projection projection_, Plato plato_,QWidget *parent)
    : QWidget{parent}
{
    drawReq.useLight = false;
    drawReq.context = this;

    setMinimumSize(600,400);

    setProjectionType(projection_);
    setProjectionPlato(plato_);
    lastTime = 0;

    Composite *XYZ = new Composite();

    XYZ->addShape(new Line(Dot(0,0,0), Dot(10,0,0), Qt::red));
    XYZ->addShape(new Line(Dot(0,0,0), Dot(0,10,0), Qt::green));
    XYZ->addShape(new Line(Dot(0,0,0), Dot(0,0,10), Qt::blue));

    void (*upAndDown)(Shape*, int) = [](Shape* shape, int step){
        qreal cycleLen = 2;
        qreal dY = 3.*4/24/cycleLen;

        step%=(int)(24*cycleLen);

        shape->move(0, dY * cos(step/(24*cycleLen)*M_PI*2), 0);
    };
    /*Composite *topSecret = new Composite();
    Composite *cube = new Composite();
    Composite *A = new Composite();
    Composite *I = new Composite();
    Composite *O = new Composite();
    Composite *L = new Composite();
    Composite *N = new Composite();
    Composite *P = new Composite();
    Composite* topSecret3D = new Composite({N,P,O,I,A,L});



    QColor color1 = Qt::magenta;
    QColor color2 = Qt::darkMagenta;
    qreal exWidth = .07;

    topSecret->addShape(new Line(Dot(-0.90, 0.25, 0), Dot(-0.90,-0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.71, 0.25, 0), Dot(-0.90, 0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.71, 0.25, 0), Dot(-0.71,-0.25, 0), color1));

    P->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[0]), exWidth, color1, color2, Ozx));
    P->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[1]), exWidth, color1, color2, Oyz));
    P->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[2]), exWidth, color1, color2, Ozx));

    //topSecret->addShape(new Line(Dot(-0.64, 0.00, 0), Dot(-0.56, 0.25, 0), color1));
    //topSecret->addShape(new Line(Dot(-0.44, 0.25, 0), Dot(-0.36, 0.00, 0), color1));
    topSecret->addShape(new Line(Dot(-0.60, 0.25, 0), Dot(-0.60,-0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.60, 0.25, 0), Dot(-0.60,-0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.60, 0.25, 0), Dot(-0.40, 0.25, 0), color1));
    //topSecret->addShape(new Line(Dot(-0.64, 0.00, 0), Dot(-0.56,-0.25, 0), color1));
    //topSecret->addShape(new Line(Dot(-0.44,-0.25, 0), Dot(-0.36,0.00, 0), color1));
    topSecret->addShape(new Line(Dot(-0.40, 0.25, 0), Dot(-0.40,-0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.40, 0.25, 0), Dot(-0.40,-0.25, 0), color1));
    topSecret->addShape(new Line(Dot(-0.60,-0.25, 0), Dot(-0.40,-0.25, 0), color1));

    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[3]), exWidth, color1, color2, Ozx));
    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[4]), exWidth, color1, color2, Ozx));
    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[5]), exWidth, color1, color2, Oyz));
    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[6]), exWidth, color1, color2, Ozx));
    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[7]), exWidth, color1, color2, Ozx));
    O->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[8]), exWidth, color1, color2, Oyz));

    topSecret->addShape(new Line(Dot(-0.30,-0.25-exWidth/2, 0), Dot(-0.17, 0.25+exWidth/2, 0),color1));
    topSecret->addShape(new Line(Dot(-0.17, 0.25+exWidth/2, 0), Dot(-0.04,-0.25-exWidth/2, 0),color1));

    L->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[9]), exWidth, color1, color2, Ozx,0));
    L->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[10]), exWidth, color1, color2, Ozx,0));

    topSecret->addShape(new Line(Dot( 0.06,-0.25, 0), Dot( 0.06, 0.25, 0),color1));
    topSecret->addShape(new Line(Dot( 0.06,-0.25-exWidth/2, 0), Dot( 0.30, 0.25+exWidth/2, 0),color1));
    topSecret->addShape(new Line(Dot( 0.30,-0.25, 0), Dot( 0.30, 0.25, 0),color1));

    I->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[11]), exWidth, color1, color2, Ozx));
    I->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[12]), exWidth, color1, color2, Ozx,0));
    I->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[13]), exWidth, color1, color2, Ozx));

    topSecret->addShape(new Line(Dot( 0.38,-0.25, 0), Dot( 0.38, 0.25, 0),color1));
    topSecret->addShape(new Line(Dot( 0.38,-0.00, 0), Dot( 0.60, 0.00, 0),color1));
    topSecret->addShape(new Line(Dot( 0.60,-0.25, 0), Dot( 0.60, 0.25, 0),color1));

    N->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[14]), exWidth, color1, color2, Ozx));
    N->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[15]), exWidth, color1, color2, Oyz));
    N->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[16]), exWidth, color1, color2, Ozx));

    topSecret->addShape(new Line(Dot( 0.68,-0.25-exWidth/2, 0), Dot( 0.81, 0.25+exWidth/2, 0),color1));
    topSecret->addShape(new Line(Dot( 0.81, 0.25+exWidth/2, 0), Dot( 0.94,-0.25-exWidth/2, 0),color1));
    topSecret->addShape(new Line(Dot((0.68+0.81)/2,-0.0,0), Dot((0.94+0.81)/2,0.00,0),color1));

    A->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[17]), exWidth, color1, color2, Ozx,0));
    A->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[18]), exWidth, color1, color2, Ozx,0));
    A->addShape(Polyedr::expandLine(dynamic_cast<Line*>(topSecret->getShapes()[19]), exWidth, color1, color2, Oyz,0));

    int topSecretSplineQuality = 50;
    Spline* Pspline = new Spline({Dot(-0.90,-0.25, 0), Dot(-0.90, 0.25, 0),
                                                          Dot(-0.71, 0.25, 0), Dot(-0.71,-0.25, 0)}, color1);
    Spline* Ospline = new Spline({Dot(-0.60,-0.25, 0), Dot(-0.60, 0.25, 0),
                                                          Dot(-0.40, 0.25, 0), Dot(-0.40,-0.25, 0)}, color1);
    Spline* OsplineExtra = new Spline({
                                                               Dot(-0.40,-0.25, 0),Dot(-0.50,-0.30, 0),Dot(-0.60,-0.25, 0)}, color1);
    Spline* Lspline = new Spline({Dot(-0.30,-0.15, 0), Dot(-0.24,-0.15, 0), Dot(-0.17, 0.25, 0),
                                                          Dot(-0.04,-0.25, 0)}, color1);
    Spline* Ispline = new Spline({Dot( 0.06, 0.25, 0), Dot( 0.06,-0.25, 0),
                                                          Dot( 0.30, 0.25, 0), Dot( 0.30,-0.25, 0)}, color1);
    Spline* Nspline = new Spline({Dot( 0.38, 0.25, 0), Dot( 0.38,-0.25, 0),
                                                          Dot( 0.38,-0.04, 0), Dot( 0.60, 0.04, 0),
                                                          Dot( 0.60, 0.25, 0), Dot( 0.60,-0.25, 0),}, color1);
    Spline* Aspline = new Spline({Dot( 0.94,-0.25, 0), Dot( 0.81, 0.25, 0),
                                                          Dot( 0.68,-0.25, 0), Dot( 0.68,-0.06, 0),
                                                          Dot( 1.00, 0.05, 0), Dot( 1.00, 0.08, 0)}, color1);



    Composite* topSecretSpline = new Composite({Pspline, Ospline, OsplineExtra, Lspline, Ispline, Nspline, Aspline});
    for (Shape* shape: topSecretSpline->getShapes()){
        dynamic_cast<Spline*>(shape)->setSubNodesCnt(topSecretSplineQuality);
    }

    Spline* tSSA = new Spline(color1);
    tSSA->setSubNodesCnt(200);
    Composite* topSecretSpline2 = new Composite({tSSA});
    tSSA->addNode(Dot(0, 0.6, 0), Vector(1,2,0));
    tSSA->addNode(Dot(1.1, 0.6, 0), Vector(0,-2,0));
    tSSA->addNode(Dot(0, -1, 0), Vector(0,-1.5,0));
    topSecretSpline2->addShape(tSSA->transformed(matrix<4,4>({
        {-1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
    })));



    Dot d1( 0.4, 0.6,-0.3);
    Dot d2( 0.7,-0.5, 0.2);
    Dot d3(-0.7,-0.2, 0.7);
    Dot d4(-0.9,-0.6,-0.7);
    Dot d5(-0.7,-0.7, 0.4);
    Dot d6(-0.5,-1.0, 0.0);

    Polyedr* polyedr = new Polyedr({Polygon ({d1,d2,d3}, Qt::red)
            ,Polygon ({d1,d4,d3}, Qt::green)
            ,Polygon ({d1,d2,d4}, Qt::cyan)
            ,Polygon ({d3,d2,d5}, Qt::blue)
            ,Polygon ({d3,d4,d5}, Qt::magenta)
            ,Polygon ({d6,d4,d5}, Qt::yellow)
            ,Polygon ({d2,d6,d5}, Qt::darkCyan)
            ,Polygon ({d2,d4,d6}, Qt::darkMagenta)
    });

    Dot t0(0,0,0);
    Dot t1(1,0,0);
    Dot t2(0,1,0);
    Dot t3(0,0,1);

    Polyedr* tetraedr = new Polyedr({Polygon ({t0,t1,t2}, Qt::cyan)
            , Polygon ({t1,t2,t3}, Qt::darkYellow)
            , Polygon ({t0,t1,t3}, Qt::blue)
            , Polygon ({t0,t2,t3}, Qt::green)
    });

    Dot q1(1,2,3);
    Dot q2(5,1,2);
    Dot q3(4,5,1);
    Dot q4(2,3,6);

    Composite* tetraedrTestCentral = new Composite();

    tetraedrTestCentral->addShape(new Line(q1,q2 Qt::darkCyan));
    tetraedrTestCentral->addShape(new Line(q2,q3 Qt::darkMagenta));
    tetraedrTestCentral->addShape(new Line(q3,q4, Qt::cyan));
    tetraedrTestCentral->addShape(new Line(q4,q1, Qt::magenta));
    tetraedrTestCentral->addShape(new Line(q1,q3 Qt::darkYellow));
    tetraedrTestCentral->addShape(new Line(q2,q4, Qt::yellow));

    Dot a1(-1,3,1);
    Dot a2(2,0,1);
    Dot a3(3,3,0);
    Dot a4(1,0,0);

    Composite* tetraedrTest2 = new Composite();


    tetraedrTest2->addShape(new Line(a1,a2));
    tetraedrTest2->addShape(new Line(a2,a3));
    tetraedrTest2->addShape(new Line(a3,a4));
    tetraedrTest2->addShape(new Line(a4,a1));
    tetraedrTest2->addShape(new Line(a1,a3));
    tetraedrTest2->addShape(new Line(a2,a4));
*/
    grid = new Composite();

    gridSize = 1;
    gridCount = 25;
    for (int i=-gridCount;i<gridCount;i++){
        grid->addShapeDel(new Line(Dot(i*gridSize,0,-gridCount*gridSize), Dot(i*gridSize,0,gridCount*gridSize), QColor(80,80,80)));
        grid->addShapeDel(new Line(Dot(-gridCount*gridSize,0,i*gridSize), Dot(gridCount*gridSize,0,i*gridSize), QColor(80,80,80)));
    }

/*
    Composite* RGBcubes = new Composite();

    int cubeCnt = 5;
    qreal cubeSize = 2.00/cubeCnt;
    qreal x0 = -1, y0 = -1, z0 = -1, x1 = 1, y1 = 1, z1 = 1;
        for (qreal y = y0; y <= y1-cubeSize+0.001; y += (y1-cubeSize-y0)/(cubeCnt-1)){
    for (qreal x = x0; x <= x1-cubeSize+0.001; x += (x1-cubeSize-x0)/(cubeCnt-1)){
            for (qreal z = z0; z <= z1-cubeSize+0.001; z += (z1-cubeSize-z0)/(cubeCnt-1)){
                QColor color;
                color.setRgb((x-x0)/(x1-cubeSize-x0)*255,(y-y0)/(y1-cubeSize-y0)*255,(z-z0)/(z1-cubeSize-z0)*255);

                Polyedr* cube = new Polyedr(Polyedr::createPrism(Dot(x, y, z)
                                                                 ,Vector(cubeSize,0,0),Vector(0,cubeSize,0),Vector(0,0,cubeSize),Qt::black));
                cube->setColor(color);
                RGBcubes->addShape(cube);
            }
        }
    }
/*
    srand(0);

    Composite* circlePolygonDots = new Composite();

    int circleCnt = 10;
    qreal endAngle = 2*M_PI;
    qreal angle = 0;
    qreal rCircle = 1;
    for (int i=0;i<circleCnt;i++, angle += endAngle/circleCnt){
        circlePolygonDots->addShape(new Dot(-rCircle*cos(angle), (qreal)i*0, -rCircle*sin(angle)));
    }

    Composite* randomDots = new Composite();

    int randomDotsCnt = 100;
    for (int i=0;i<randomDotsCnt;i++){
        qreal x =(rand()%20001)/10000.0-1;
        qreal z =(rand()%20001)/10000.0-1;
        Dot* dot = new Dot(x, 0, z);
        randomDots->addShape(dot);
    }

    Polygon* starRandom = new Polygon();

    qreal rStarMax = 1;
    qreal rStarMin = 0.1;
    qreal dAngleMax = M_PI/24;
    qreal dAngleMin = M_PI/36;
    for (qreal angleStar = 0; angleStar < M_PI*2; angleStar += (rand()%100+1)/100.0*(dAngleMax-dAngleMin)+dAngleMin){
        qreal r = (rand()%100+1)/100.0*(rStarMax-rStarMin)+rStarMin;
        starRandom->addDot(Dot(r*sin(angleStar), 0, r*cos(angleStar)));
    }

    Polygon* star = new Polygon();
    int starCnt = 5;
    qreal angleStar = 0;
    qreal rStarOut = 1;
    qreal rStarIn = 0.35;
    for (int i=0;i<starCnt;i++, angleStar += M_PI*2/starCnt/2){
        star->addDot(Dot(rStarOut*sin(angleStar), (qreal)i*0, rStarOut*cos(angleStar)));
        angleStar += M_PI*2/starCnt/2;
        star->addDot(Dot(rStarIn*sin(angleStar), (qreal)i*0, rStarIn*cos(angleStar)));
    }


 */

        void (*spinY)(Shape*, int) = [](Shape* shape, int){
        qreal dAngleB = M_PI/2 / 24;
        shape->rotateY(dAngleB);
    };
/*    void (*spinYwithSplit)(Shape*, int) = [](Shape* shape_, int){
        qreal dAngleB = M_PI/2 / 24;

        if (typeid(*shape_) == typeid(Composite)){
            Composite* composite = dynamic_cast<Composite*>(shape_);
            int cnt = 0;
            for (Shape* shape: composite->getShapes()){
                matrix<4,4> FtoCenter({
                                        {1,0,0,shape->getCenter().getX()},
                                        {0,1,0,shape->getCenter().getY()},
                                        {0,0,1,shape->getCenter().getZ()},
                                        {0,0,0,1},
                                        });
                int ed = ((cnt++/16)%2 == 0?1:-1);
                matrix<4,4> Fspin({
                                    { cos(dAngleB*ed),0,sin(dAngleB*ed),0},
                                    {0,            1,0,           0},
                                    {-sin(dAngleB*ed),0,cos(dAngleB*ed),0},
                                    {0,0,0,1},
                                    });
                matrix<4,4> FfromCenter({
                                          {1,0,0,-shape->getCenter().getX()},
                                          {0,1,0,-shape->getCenter().getY()},
                                          {0,0,1,-shape->getCenter().getZ()},
                                          {0,0,0,1},
                                          });
                shape->transform(FtoCenter*Fspin*FfromCenter);
            }
        }
    };

    int letterWaveDelay = 5;

    for (int i=0;i<topSecret3D->getShapes().size();i++) {
        topSecret3D->getShapes()[i]->addAnimation(upAndDown, letterWaveDelay*i);
        topSecret3D->getShapes()[i]->addAnimation(spinY, letterWaveDelay*i);
    }

    //cube->addAnimation(upAndDown);
    //cube->addAnimation(spinY);

    RGBcubes->addAnimation(spinYwithSplit);

    matrix<4,4> T;
    T[0][3] = -2;
    Composite* RGBcubes2(RGBcubes->transformed(T));
    RGBcubes2->addAnimation(spinY);
    T[0][3] = 2;
    RGBcubes->transform(T);
*/
        /*
    Composite* top = new Composite();
    Composite* bot = new Composite();
    Composite* rig = new Composite();
    Composite* lef = new Composite();
    Composite* fac = new Composite();
    Composite* bac = new Composite();
    Composite* cubicRubic = new Composite({top, bot, rig, lef, fac, bac});

    //ROGBYW;
    for (int xi=0;xi<3;xi++){
        for (int yi=0;yi<3;yi++){
            for (int zi=0;zi<3;zi++){
                Polyedr subCube = Polyedr::createPrism(Dot((xi-1.5)*cubicRubicSize/3,(yi-1.5)*cubicRubicSize/3,(zi-1.5)*cubicRubicSize/3)
                                                ,Vector(cubicRubicSize/3,0,0),Vector(0,cubicRubicSize/3,0),Vector(0,0,cubicRubicSize/3));

                for (Polygon& edge: subCube.getPolygons()){
                    Vector n = subCube.getNormalOut(edge);
                    if (n.getX() > 0 && xi == 2){
                        edge.setColor(Qt::red);
                    }
                    if (n.getX() < 0 && xi == 0){
                        edge.setColor(Qt::magenta);
                    }
                    if (n.getY() > 0 && yi == 2){
                        edge.setColor(Qt::green);
                    }
                    if (n.getY() < 0 && yi == 0){
                        edge.setColor(Qt::yellow);
                    }
                    if (n.getZ() > 0 && zi == 2){
                        edge.setColor(Qt::blue);
                    }
                    if (n.getZ() < 0 && zi == 0){
                        edge.setColor(Qt::cyan);
                    }
                }

                if (xi == 0){
                    lef->addShape(&subCube);
                }
                if (xi == 2){
                    rig->addShape(&subCube);
                }
                if (yi == 0){
                    bot->addShape(&subCube);
                }
                if (yi == 2){
                    top->addShape(&subCube);
                }
                if (zi == 0){
                    fac->addShape(&subCube);
                }
                if (zi == 2){
                    bac->addShape(&subCube);
                }
            }
        }
    }


/*
    SplineErmit* spline1 = new SplineErmit();
    spline1->setSubNodes(1000);

    spline1->addNode(Dot(0,0,0));
    spline1->addNode(Dot(1,0,0));
    spline1->addNode(Dot(1,1,0));
    spline1->addNode(Dot(2,2,0));
    spline1->addNode(Dot(2,3,0));


    SplineErmit* splineErmit = new SplineErmit({Dot(-1,0,0, Qt::red), Dot(0,1,0, Qt::yellow), Dot(1,0,0, Qt::blue)});
    splineErmit->setSubNodes(100);
    splineErmit->setVector(0, Vector(0,-10,0));
    splineErmit->setVector(1, Vector(0,0,10));
    splineErmit->setVector(2, Vector(0, 10,0));




    qreal k2 = 1.565;
    SplineErmit main1({Dot(0,-1,0), Dot(0,1,0)});
    SplineErmit dir1({Dot(-1,0,-1),Dot(1,0,-1),Dot(1,0,1),Dot(-1,0,1),Dot(-1,0,-1)});

    dir1.setVector(0, Vector( k2,0,-k2));
    dir1.setVector(1, Vector( k2,0, k2));
    dir1.setVector(2, Vector(-k2,0, k2));
    dir1.setVector(3, Vector(-k2,0,-k2));
    dir1.setVector(4, Vector( k2,0,-k2));



    SurfaceKinematic* surfk1 = new SurfaceKinematic(main1, dir1, 0);
    surfk1->setSubNodesDir(64);
    surfk1->setSubNodesMain(4);


    SplineErmit main2({Dot(-1,0,0), Dot(1,0,0)});
    SplineErmit dir2;

    int waveCnt = 4;
    qreal len = waveCnt*2;
    for (int i=0;i<waveCnt;i++){
        dir2.addNode(Dot(0, 0, (4.*i)/(4*(waveCnt))*len-len/2));
        dir2.addNode(Dot(0, 1, (4.*i+1)/(4*(waveCnt))*len-len/2));
        dir2.addNode(Dot(0, 0, (4.*i+2)/(4*(waveCnt))*len-len/2));
        dir2.addNode(Dot(0, -1, (4.*i+3)/(4*(waveCnt))*len-len/2));
    }
    dir2.addNode(Dot(0, 0, len/2));

    SurfaceKinematic* surfk2 = new SurfaceKinematic(main2, dir2, 0);
    surfk2->setSubNodesDir(256);
    surfk2->setSubNodesMain(2);


    dir2.setColor(Qt::white);
    dir2.setSubNodes(300);


*/
/*
    SplineErmit dir3({Dot(0,0,0)});
    SplineErmit* main3 = new SplineErmit({Dot(0,0,0), Dot(.8,.8,0), Dot(0.2,2,0)});
    main3->setName("main3");
    main3->move(0,-1,0);
    main3->setVector(0, Vector(2,0,0));
    main3->setVector(1, Vector(-0.5,1,0));
    main3->setVector(2, Vector(0,2,0));
    main3->setSubNodes(160);

    Dot (*F3)(Dot, qreal, qreal) = [](Dot dot, qreal t1, qreal t2){
        dot.rotateY(M_PI*2*t2);
    //    dot.move(Vector(1,0,0)*t1);
        return dot;
    };

    SurfaceKinematic* surfk3 = new SurfaceKinematic(*main3, dir3, F3);
    surfk3->setSubNodesDir(16);
    surfk3->setSubNodesMain(16);



    SplineErmit dir4({Dot(0,0,0)});
    SplineErmit main4({Dot(0,0.3,0), Dot(.6,1.2,0), Dot(0.25,2.6,0)});
    main4.setName("main4");
    main4.move(0,-1,0);
    main4.setVector(0, Vector(1,-2,0));
    main4.setVector(1, Vector(-0.3,0.4,0));
    main4.setVector(2, Vector(1.2,2.5,0));
    main4.setSubNodes(160);

    Dot (*F4)(Dot, qreal, qreal) = [](Dot dot, qreal t1, qreal t2){
        dot.rotateY(M_PI*2*t2);
        return dot;
    };
    main4.setCenter(Dot(0,0,0));
    SurfaceKinematic* surfk4 = new SurfaceKinematic(main4, dir4, F4);
    surfk4->setSubNodesDir(20);
    surfk4->setSubNodesMain(24);

    surfk4->setColor(Qt::white);
//    surfk4->rotateZ(M_PI/6);
    surfk4->borderOn();

   // surfk4->addAnimation(upAndDown);
/*
    Polygon* poly1 = new Polygon({Dot(1, -2, -1), Dot(1, 2, -1), Dot(1, 2, 1), Dot(1, -2, 1)});
    Polygon* poly2 = new Polygon({Dot(1, 0, -1),  Dot(1, 2, 1), Dot(1, -2, 1)});


    Line* l1 = new Line(Dot(1,2,1, Qt::red), Dot(1,0,-1, Qt::green));


    Polyedr* c2 = new Polyedr(Polyedr::createPrism(Dot(-1,-1,-1), Vector(2,0,0), Vector(0,2,0), Vector(0,0,2)));
    Composite* cube2 = new Composite({new Polyedr(Polyedr::createPrism(Dot(-1,-1,-1), Vector(2,0,0), Vector(0,2,0), Vector(0,0,2)))});
    delete c2;
    cube2->addAnimation(upAndDown);
    cube2->addAnimation(spinY);
*/
/*

    SplineKinematic dir_sk0(Dot(3,0,0), [](Dot dot, qreal t){
        dot.rotateY(t*2*M_PI);
        return dot;
    });
    SplineKinematic main_sk0(Dot(0,.4,0), [](Dot dot, qreal t){
        dot.rotateZ(t*2*M_PI);
        return dot;
    });

    Dot (*F_sk0)(Dot, qreal, qreal) = [](Dot dot, qreal t1, qreal t2){
        dot.rotateY(M_PI*2*t2);
        return dot;
    };

    SurfaceKinematic* sk0 = new SurfaceKinematic(main_sk0, dir_sk0, F_sk0);
    sk0->setSubNodesDir(24);
    sk0->setSubNodesMain(16);



    SplineKinematic dir_sk0_1(Dot(3,0,0), [](Dot dot, qreal t){
        dot.rotateY(t*2*M_PI);
        return dot;
    });
    SplineKinematic main_sk0_1(Dot(0,.1,0), [](Dot dot, qreal t){
        dot.rotateZ(t*2*M_PI);
        return dot;
    });

    Dot (*F_sk0_1)(Dot, qreal, qreal) = [](Dot dot, qreal t1, qreal t2){

        dot.rotateY(M_PI*2*t2);
        Dot extra(0, 0.5, 0);
        qreal k = 1;
        extra.setCenter(Dot(0,0,0));
        extra.rotateZ(t2*2*M_PI*k);
        extra.rotateY(t2*2*M_PI);


        return dot+extra;
    };

    SurfaceKinematic* sk0_1 = new SurfaceKinematic(main_sk0_1, dir_sk0_1, F_sk0_1);
    sk0_1->setSubNodesMain(5);
    sk0_1->setSubNodesDir(80);

    Composite* sk01 = new Composite({sk0, sk0_1});


    qreal d = 1;
    SurfaceErmit* se0 = new SurfaceErmit(Dot(-d,-d*0,-d), Dot(-d,-d*0,d), Dot(d,-d*0,-d), Dot(d,-d*0,d));
    se0->setSubNodes(24);

    se0->setVecRot(0, Vector(0,40,0));



    SurfaceInterpolated* si0 = new SurfaceInterpolated({
        {Dot(-1,0,-1,Qt::green),Dot(-1,0,0),Dot(-1,0,1),},
        {Dot(0,0,-1),Dot(0,1,0, Qt::red),Dot(0,0,1),},
        {Dot(1,0,-1),Dot(1,0,0),Dot(1,0,1,Qt::blue),},
    });
    si0->setSubNodes(24);
    si0->borderOn();

        /*
    SurfaceInterpolated* rgbTriangle = new SurfaceInterpolated({
        {Dot(0,0,-1,Qt::green),Dot(0,0,-1,Qt::green),Dot(0,0,-1,Qt::green),},
        {Dot(0.5,0,0, Qt::yellow),Dot(0,0,0, Qt::white),Dot(-0.5,0,0,Qt::cyan),},
        {Dot(1,0,1,Qt::red),Dot(0,0,1,Qt::magenta),Dot(-1,0,1,Qt::blue),},
        });
    rgbTriangle->setSubNodes(32);
    rgbTriangle->borderOff();



    Composite* cube = new Composite();

    Dot u000(1,1,1, Qt::red);
    Dot u001(1,1,-1);
    Dot u010(1,-1,1);
    Dot u011(1,-1,-1);
    Dot u100(-1,1,1);
    Dot u101(-1,1,-1);
    Dot u110(-1,-1,1);
    Dot u111(-1,-1,-1);

    Polyedr p0;

    p0.addPolygon(Polygon({u000,u001,u101,u100}));
    p0.addPolygon(Polygon({u000,u010,u011,u001}));
    p0.addPolygon(Polygon({u000,u100,u110,u010}));
    p0.addPolygon(Polygon({u111,u011,u010,u110}));
    p0.addPolygon(Polygon({u111,u101,u001,u011}));
    p0.addPolygon(Polygon({u111,u110,u100,u101}));

    cube->addShape(&p0);


    SurfaceErmit* surf1 = new SurfaceErmit(Dot(-1,2,-1),Dot(-1,0,1),Dot(1,0,-1),Dot(1,2,1));
    surf1->setSubNodes(16);

    /*qreal k1 = 1;
    surf1->setNode(0, Dot(-1,0,-1), Vector(k1, 0,-k1), Vector(-k1, 0, k1), Vector(-20,-10,-20));
    surf1->setNode(1, Dot(-1,0, 1), Vector(k1, 0, k1), Vector( k1, 0, k1), Vector( 20, 10,-20));
    surf1->setNode(2, Dot( 1,0,-1), Vector(k1, 0, k1), Vector( k1, 0, k1), Vector(-20, 10, 20));
    surf1->setNode(3, Dot( 1,0, 1), Vector(k1, 0,-k1), Vector(-k1, 0, k1), Vector( 20,-10, 20));

    */
/*
    SurfaceInterpolated* si1 = new SurfaceInterpolated({
        {Dot(-2,0,2),Dot(-1,0.2,2),Dot(0,0,2),Dot(1,-0.3,2),Dot(2,0,2)},
        {Dot(-2,0.6,1),Dot(-1,0,1),Dot(0,0,1),Dot(1,0,1),Dot(2,-0.2,1)},
        {Dot(-2,0,0),Dot(-1,0,0),Dot(0,0,0),Dot(1,0.7,0),Dot(2,0.1,0)},
        {Dot(-2,0,-1),Dot(-1,0,-1),Dot(0,-0.4,-1),Dot(1,0,-1),Dot(2,-0.3,-1)},
        {Dot(-2,0,-2),Dot(-1,0,-2),Dot(0,-0.4,-2),Dot(1,0.2,-2),Dot(2,0.3,-2)},
    });
    si1->setSubNodes(24);




    SurfaceInterpolated* si2 = new SurfaceInterpolated({
        {Dot(1,0,1),Dot(1,0,0),Dot(1,0,-1),},
        {Dot(0,0,1),Dot(0,-2,0),Dot(0,0,-1),},
        {Dot(-1,0,1),Dot(-1,0,0),Dot(-1,0,-1),},
    });
    si2->setSubNodes(16);

    si2->setVecRot(0,0,Vector(0,20,0));




        SplineErmit* PSpline = new SplineErmit();
        PSpline->addNode(Dot(-1,-1,0),Vector(0,1,0));
        PSpline->addNode(Dot(-1,1,0),Vector(1,0,0));
        PSpline->addNode(Dot(1,1,0),Vector(1,0,0));
        PSpline->addNode(Dot(1,-1,0),Vector(0,-1,0));
        PSpline->setSubNodes(100);
        PSpline->scale(0.5,1,1);



*/


    SplineErmit* DSpline = new SplineErmit();
    DSpline->addNode(Dot(0,0,0),Vector(0,-9,0));
    DSpline->addNode(Dot(0,-0.8,0),Vector(1.3,-0.4,0));
    DSpline->addNode(Dot(0.7,-0.4,0),Vector(0,1.4,0));
    DSpline->addNode(Dot(-0.3,-0.3,0),Vector(0.8,-4,0));
    DSpline->setSubNodes(100);


    SplineKinematic* sq = new SplineKinematic(Dot(0,0,0), [](Dot dot, qreal t){;
        if (t < 1){
            return dot + Dot(-1,0,-1) + t * Dot(0,0,2);
        }
        else if (t < 2){
            return dot +Dot(-1,0,1) + (t-1) * Dot(2,0,0);
        }
        else if (t < 3){
            return dot +Dot(1,0,1) + (t-2) * Dot(0,0,-2);
        }
        else{
            return dot +Dot(1,0,-1) + (t-3) * Dot(-2,0,0);
        }
    }, 4);
    sq->setSubNodes(5);
    sq->scale(0.1,1,0.1);
    sq->rotateZ(M_PI/6);

    SurfaceKinematic* DSurf = new SurfaceKinematic(*sq, *DSpline);
    DSurf->setSubNodesDir(100);
    DSurf->setSubNodesMain(5);
    DSurf->move(Vector(0,0.5,0));
    DSurf->polygonBackOff();
    DSurf->borderOff();

/*    SurfaceKinematic* PSurf = new SurfaceKinematic(*sq, *PSpline);
    PSurf->setSubNodesDir(100);
    PSurf->setSubNodesMain(5);
    PSurf->move(Vector(0,0.5,0));
    PSurf->polygonBackOff();
    PSurf->borderOff();
    //shapes.push_back(sq);
*/
    Dot light0(2,2,2);
    light0.setColor(Qt::white);
    drawReq.lightDots.push_back({light0, 100});
    Dot light20(-2,-2,-2);
    light20.setColor(Qt::white);
    drawReq.lightDots.push_back({light20, 100});
//    Dot light00(-2,-2,-2);
//    light00.setColor(Qt::white);
//    drawReq.lightDots.push_back({light00, 100});
//   Dot light1(-2,-2,2);
//   light1.setColor(Qt::cyan);
//   drawReq.lightDots.push_back({light1, 30});
//   Dot light2(-2,-2,-2);
//   light2.setColor(Qt::yellow);
//   drawReq.lightDots.push_back({light2, 30});

   Dot light3(0,2,0);
   light3.setColor(Qt::magenta);
  // drawReq.lightDots.push_back({light3, 100});


    //drawReq.lights.push_back(Vector(0,1,0));

    //drawReq.lightDots.push_back({Dot(0,5,0),200});
    //drawReq.lightDots[0].first.addAnimation(upAndDown);

    drawReq.useLight = true;





    shapes.push_back(grid);
    shapes.push_back(XYZ);
    //    shapes.push_back(PSurf);
    shapes.push_back(DSurf);
    //shapes.push_back(DSpline);
 //   shapes.push_back(surf1);
    //shapes.push_back(si2);
    //shapes.push_back(si1);
   // shapes.push_back(rgbTriangle);
//    shapes.push_back(si0);
 //   shapes.push_back(sk01);
//    shapes.push_back(sk0_1);
//    shapes.push_back(sk0);
//    shapes.push_back(se0);
//    shapes.push_back(cube);
//    shapes.push_back(l1);
//    shapes.push_back(poly2);
//    shapes.push_back(surfk4);
//    shapes.push_back(main3);
//    shapes.push_back(surfk3);
//    shapes.push_back(surfk2);
//    shapes.push_back(surfk1);
//    shapes.push_back(dir2);
//    shapes.push_back(splineErmit);
//    shapes.push_back(surf1);
//    shapes.push_back(spline1);
 //   shapes.push_back(cubicRubic);
//    shapes.push_back(starRandom->getTringulation());
//    shapes.push_back(star->getTringulation());
//    shapes.push_back(randomDots->getTriangulationDelone());
//    shapes.push_back(circlePolygonDots->getConvexBound()->getTringulation());
//    shapes.push_back(cube);
//    shapes.push_back(tetraedrTestCentral);
//    shapes.push_back(tetraedrTest2);
//    shapes.push_back(tetraedr);
//    shapes.push_back(polyedr);
//    shapes.push_back(RGBcubes);
//    shapes.push_back(topSecret);
//    shapes.push_back(topSecret3D);
//    shapes.push_back(topSecretSpline);
  //  shapes.push_back(topSecretSpline2);

    zoom = 0.6;

    angleAlpha = M_PI/4; // Ox
    angleBeta =- M_PI / 6; // Oy
    angleGamma = M_PI * 0; // Oz

    onViewer.setColumn(0, {1, 1, 1});


    drawReq.onViewer = onViewer;
    drawReq.zoom = zoom;

    updateF();
}
void AreaPaint::setFAksonometric(){

    matrix<4,4> basis;

    basis.setRow(0, {(double)((plato&1?1:0)), 0, 0});
    basis.setRow(1, {0, (double)(((plato&2)!=0)), 0});
    basis.setRow(2, {0, 0, (double)(((plato&4)!=0))});

    matrix<4,4,qreal> mAlpha, mBeta, mGamma;
    mGamma.setRow(0, {cos(angleGamma), -sin(angleGamma), 0});
    mGamma.setRow(1, {sin(angleGamma),  cos(angleGamma), 0});

    mAlpha.setRow(1, {0, cos(angleAlpha),-sin(angleAlpha)});
    mAlpha.setRow(2, {0, sin(angleAlpha), cos(angleAlpha)});

    mBeta.setRow(0, { cos(angleBeta), 0, sin(angleBeta)});
    mBeta.setRow(2, {-sin(angleBeta), 0, cos(angleBeta)});

    matrix<4,4> F(mGamma*mAlpha*mBeta);
    basis*=F;

    matrix<4,1> onViewerTemp;
    onViewerTemp.setColumn(0, {(double)((plato == Oyz)),(double)(plato == Ozx),(double)(plato == Oxy), 0});

    onViewerTemp = F.inverted() * onViewerTemp;


    drawReq.F = basis;
    drawReq.onViewer = Vector(onViewerTemp[0][0], onViewerTemp[1][0], onViewerTemp[2][0]);


}
void AreaPaint::setFAskew(){
    matrix<4,4, qreal> F;
    if (plato == Oxy){
        if (onViewer[2][0] == 0){
            qDebug()<<"Z zeroDiv askew paint";
            throw 1;
        }

        F.setRow(0,{sgn(onViewer[2][0]), 0,sgn(onViewer[2][0]) * -onViewer[0][0]/onViewer[2][0]});
        F.setRow(1,{0, 1, -onViewer[1][0]/onViewer[2][0]});
        F.setRow(2,{0, 0, 0});

    }
    if (plato == Oyz){
        if (onViewer[0][0] == 0){
            qDebug()<<"X zeroDiv askew paint";
            throw 1;
        }

        F.setRow(0,{0, 0, 0});
        F.setRow(1,{sgn(onViewer[0][0])* -onViewer[1][0]/onViewer[0][0], sgn(onViewer[0][0]), 0});
        F.setRow(2,{-onViewer[2][0]/onViewer[0][0], 0, 1});

    }
    if (plato == Ozx){
        if (onViewer[1][0] == 0){
            qDebug()<<"Y zeroDiv askew paint";
            throw 1;
        }

        F.setRow(0,{1, -onViewer[0][0]/onViewer[1][0], 0});
        F.setRow(1,{0, 0, 0});
        F.setRow(2,{0, sgn(onViewer[1][0])* -onViewer[2][0]/onViewer[1][0], sgn(onViewer[1][0])});

    }
    drawReq.F = F;
    drawReq.onViewer = onViewer;
}
void AreaPaint::setFCentral(){
    matrix<4,4> F;
    if (plato == Oxy){
        if (onViewer[2][0] == 0){
            qDebug()<<"Z zeroDiv central paint";
            throw 1;
        }

        qreal ve = sgn(onViewer[2][0])*onViewer[2][0];
        F.setRow(0,{ve, 0, -onViewer[0][0]*sgn(onViewer[2][0]), 0});
        F.setRow(1,{0, ve, -onViewer[1][0]*sgn(onViewer[2][0]), 0});
        F.setRow(2,{0, 0, 0, 0});
        F.setRow(3,{0, 0, -sgn(onViewer[2][0]), ve});

    }
    if (plato == Oyz){
        if (onViewer[0][0] == 0){
            qDebug()<<"X zeroDiv central paint";
            throw 1;
        }
        qreal ve = sgn(onViewer[0][0])*onViewer[0][0];
        F.setRow(0,{0, 0, 0, 0});
        F.setRow(1,{-onViewer[1][0]*sgn(onViewer[0][0]), ve, 0, 0});
        F.setRow(2,{-onViewer[2][0]*sgn(onViewer[0][0]), 0, ve, 0});
        F.setRow(3,{-sgn(onViewer[0][0]), 0, 0, ve});

    }
    if (plato == Ozx){
        if (onViewer[1][0] == 0){
            qDebug()<<"Y zeroDiv central paint";
            throw 1;
        }
        qreal ve = sgn(onViewer[1][0])*onViewer[1][0];
        F.setRow(0,{ve, -onViewer[0][0]*sgn(onViewer[1][0]), 0, 0});
        F.setRow(1,{0, 0, 0, 0});
        F.setRow(2,{0, -onViewer[2][0]*sgn(onViewer[1][0]), ve, 0});
        F.setRow(3,{0, -sgn(onViewer[1][0]), 0, ve});

    }

    drawReq.F = F;
    drawReq.viewer = Dot(0,0,0) + onViewer;
}
void AreaPaint::setFPerspective(){


    drawReq.viewer = (Dot(0,0,0) + onViewer);

    matrix<4,4> M({
        {1,0,0,-drawReq.viewer.getX()},
        {0,1,0,-drawReq.viewer.getY()},
        {0,0,1,-drawReq.viewer.getZ()-1},
        {0,0,0,1},
    });


    matrix<4,4,qreal> mAlpha, mBeta, mGamma;
    mGamma.setRow(0, {cos(angleGamma), -sin(angleGamma), 0});
    mGamma.setRow(1, {sin(angleGamma),  cos(angleGamma), 0});

    mAlpha.setRow(1, {0, cos(angleAlpha),-sin(angleAlpha)});
    mAlpha.setRow(2, {0, sin(angleAlpha), cos(angleAlpha)});

    mBeta.setRow(0, { cos(angleBeta), 0, sin(angleBeta)});
    mBeta.setRow(2, {-sin(angleBeta), 0, cos(angleBeta)});

    matrix<4,4> R(mGamma * mAlpha * mBeta);
    R = Dot(0,0,-1).getLocalTransformation(R);

    matrix<4,4, qreal> F({
        {-1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 1, 1}
    });

    drawReq.F = F * R * M;
}
void AreaPaint::paintEvent(QPaintEvent*){
    //qDebug()<<"alpha:"<<angleAlpha*180/M_PI;
    //qDebug()<<"beta:"<<angleBeta*180/M_PI;
    //qDebug()<<"gamma:"<<angleGamma*180/M_PI;
    //qDebug()<<drawReq.onViewer.getX()<<drawReq.onViewer.getY()<<drawReq.onViewer.getZ();
    for (const Shape* shape: shapes){
        shape->draw(drawReq);
    }
}
void AreaPaint::updateF(){
    if (projection == Aksonometric){
        setFAksonometric();
    }
    else if (projection == Askew){
        setFAskew();
    }
    else if (projection == Central){
        setFCentral();
    }
    else if (projection == Perspective){
        setFPerspective();
    }
    /*qDebug();
    for (int i=0;i<4;i++){
        QString s;
        for (int j=0;j<4;j++){
            s+=QString::number(drawReq.F[i][j])+" ";
        }
        qDebug()<<s;
    }
    qDebug()<<drawReq.viewer.getX()<<drawReq.viewer.getY()<<drawReq.viewer.getZ();
*/
    update();
}
void AreaPaint::setProjectionType(Projection projection_){
    projection = projection_;
    drawReq.projection = projection;
    updateF();
}
void AreaPaint::setProjectionPlato(Plato plato_){
    plato = plato_;
    drawReq.plato = plato_;
    updateF();
}
void AreaPaint::setGridPlato(Plato plato_){
    grid->clearShapes();
    for (int i=-gridCount;i<gridCount;i++){
        for (int j=-gridCount;j<gridCount;j++){
            if (plato_ == Ozx){
                grid->addShape(new Line(Dot(i*gridSize,0,j*gridSize), Dot((i+1)*gridSize,0,j*gridSize),QColor(80,80,80)));
                grid->addShape(new Line(Dot(i*gridSize,0,j*gridSize), Dot(i*gridSize,0,(j+1)*gridSize),QColor(80,80,80)));
            }
            if (plato_ == Oyz){
                grid->addShape(new Line(Dot(0,i*gridSize,j*gridSize), Dot(0,(i+1)*gridSize,j*gridSize),QColor(80,80,80)));
                grid->addShape(new Line(Dot(0,i*gridSize,j*gridSize), Dot(0,i*gridSize,(j+1)*gridSize),QColor(80,80,80)));
            }
            if (plato_ == Oxy){
                grid->addShape(new Line(Dot(i*gridSize,j*gridSize,0), Dot((i+1)*gridSize,j*gridSize,0),QColor(80,80,80)));
                grid->addShape(new Line(Dot(i*gridSize,j*gridSize,0), Dot(i*gridSize,(j+1)*gridSize,0),QColor(80,80,80)));
            }
        }
    }

    update();
}

void AreaPaint::keyPressEvent(QKeyEvent* e){
    long long curTime = QDateTime::currentMSecsSinceEpoch();
    //qDebug()<<"FPS:"<<1000/(curTime-lastTime+1);
    lastTime = curTime;
    int key = e->key();
    int vInv = 1;
    int hInv = -1;
    qreal step = M_PI / 60;
    qreal stepAskew = .1;
    //qDebug()<<key;
    if (key == 16777234 || key == Qt::Key_A){ // left arrow
        onViewer[0][0] -= stepAskew;
    }
    else if (key == 16777236 || key == Qt::Key_D){ // right arrow
        onViewer[0][0] += stepAskew;
    }
    else if (key == 16777235 || key == Qt::Key_W){ // up arrow
        onViewer[2][0] += stepAskew;
    }
    else if (key == 16777237 || key == Qt::Key_S){ // down arrow
        onViewer[2][0] -= stepAskew;
    }
    else if (key == Qt::Key_Q){
        onViewer[1][0]-=stepAskew;
    }
    else if (key == Qt::Key_E){
        onViewer[1][0]+=stepAskew;
    }
    bool isShift = e->modifiers() & Qt::ShiftModifier;
    if (!isShift && key == Qt::Key_I){
        angleBeta += step * hInv;
    }
    if (isShift && key == Qt::Key_I){
        angleBeta -= step * hInv;
    }
    if (!isShift && key == Qt::Key_O){
        angleAlpha += step * vInv;
    }
    if (isShift && key == Qt::Key_O){
        angleAlpha -= step * vInv;
    }
    if (!isShift && key == Qt::Key_P){
        angleGamma += step;
    }
    if (isShift && key == Qt::Key_P){
        angleGamma -= step;
    }

    if (shapes.size()>2){

        qreal stp = 0.1;
        if (!isShift && key == Qt::Key_Z){
            shapes[2]->move(stp,0,0);
        }
        if (isShift && key == Qt::Key_Z){
            shapes[2]->move(-stp,0,0);
        }
        if (!isShift && key == Qt::Key_X){
            shapes[2]->move(0,stp,0);
        }
        if (isShift && key == Qt::Key_X){
            shapes[2]->move(0,-stp,0);
        }
        if (!isShift && key == Qt::Key_C){
            shapes[2]->move(0,0,stp);
        }
        if (isShift && key == Qt::Key_C){
            shapes[2]->move(0,0,-stp);
        }
        qreal ang = M_PI/12;
        if (!isShift && key == Qt::Key_V){
            shapes[2]->rotateX(ang);
        }
        if (isShift && key == Qt::Key_V){
            shapes[2]->rotateX(-ang);
        }
        qreal dSz = 0.95;
        if (!isShift && key == Qt::Key_B){
            shapes[2]->scale(dSz,1,1);
        }
        if (isShift && key == Qt::Key_B){
            shapes[2]->scale(1/dSz,1,1);
        }

    }
    qreal dx = cos(angleAlpha) * sin(-angleBeta);
    qreal dy = sin(angleAlpha);
    qreal dz = cos(angleAlpha) * cos(-angleBeta);
    if (key == Qt::Key_F){

        onViewer[0][0] += dx * stepAskew * (isShift?-1:1);
        onViewer[1][0] += dy * stepAskew * (isShift?-1:1);
        onViewer[2][0] += dz * stepAskew * (isShift?-1:1);
    }


    updateF();
}
void AreaPaint::wheelEvent(QWheelEvent* e){
    qreal angle = e->angleDelta().y();
    qreal dz = 1.05;
    if (angle > 0){
        zoom*=dz;
    }
    else if (angle < 0){
        zoom/=dz;
    }
    drawReq.zoom = zoom;
    update();
}
void AreaPaint::timerEvent(QTimerEvent *e){
    if (e->timerId() == myTimer){
        update();
    }
    /*if (e->timerId() == myTimer){
        for (Shape* shape: shapes){
            shape->animate();
        }
        for (QPair<Dot, qreal>& p: drawReq.lightDots){
            p.first.animate();
          //  p.first.show();
        }
        update();
    }*/
}
void AreaPaint::showEvent(QShowEvent *) {
    myTimer = startTimer(1000/24);
}
void AreaPaint::hideEvent(QHideEvent *){
    killTimer(myTimer);
}

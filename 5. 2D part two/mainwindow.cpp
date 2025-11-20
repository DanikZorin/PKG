#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    central = new MyCentral(this);
    setCentralWidget(central);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* menuFile = new QMenu("Файл");
    menuBar->addMenu(menuFile);

    QAction* actionOpen = new QAction("Открыть...");
    menuFile->addAction(actionOpen);
    actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    connect(actionOpen, &QAction::triggered, this, &MainWindow::OpenFile);


    menuFile->addSeparator();

    QAction* actionClear = new QAction("Стереть");
    menuFile->addAction(actionClear);
    actionClear->setShortcut(Qt::Key_Delete);
    connect(actionClear, &QAction::triggered, [=](){

        for (Shape* shape: central->shapes){
            delete shape;
        }
        central->shapes.clear();
        for (Shape* shape: central->cutted){
            delete shape;
        }
        central->cutted.clear();

        if (central->cutter){
            delete central->cutter;
            central->cutter = nullptr;
        }
        central->update();
    });

    QAction* actionUncut = new QAction("Убрать отсечения");
    menuFile->addAction(actionUncut);
    connect(actionUncut, &QAction::triggered, [=](){

        for (Shape* shape: central->cutted){
            delete shape;
        }
        central->cutted.clear();
        central->update();
    });

    menuFile->addSeparator();

    QAction* actionExit = new QAction("Выход");
    menuFile->addAction(actionExit);
    actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);


    QMenu* menuView = new QMenu("Вид");
    menuBar->addMenu(menuView);

    QAction* actionToggleGrid = new QAction("Видимость сетки");
    menuView->addAction(actionToggleGrid);
    actionToggleGrid->setCheckable(true);
    actionToggleGrid->setChecked(true);
    connect(actionToggleGrid, &QAction::toggled, [=](bool state){central->drawReq.isGrid = state; central->update();});


    QMenu* menuAlgorithm = new QMenu("Алгоритм");
    menuBar->addMenu(menuAlgorithm);

    QAction* actionKoen = new QAction("Сезерленд-Коэн");
    menuAlgorithm->addAction(actionKoen);
    connect(actionKoen, &QAction::triggered, central, &MyCentral::Koen);

    QAction* actionBarsky = new QAction("Лианга-Барски");
    menuAlgorithm->addAction(actionBarsky);
    connect(actionBarsky, &QAction::triggered, central, &MyCentral::Barsky);

    QAction* actionHodgeman = new QAction("Сазерленд-Ходжман");
    menuAlgorithm->addAction(actionHodgeman);
    connect(actionHodgeman, &QAction::triggered, central, &MyCentral::Hodgeman);


    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    mousePosLabel = new QLabel("");
    statusBar->addWidget(mousePosLabel);


    setWindowTitle("2D Part two");

    setMouseTracking(true);
    resize(600, 400);
}
#include <fstream>
void MainWindow::OpenFile(){
    QString fileName = QFileDialog::getOpenFileName(this, "", QDir::homePath(), " *.txt ;; * ");

    if (!fileName.isNull()){
        for (Shape* shape:central->shapes)
            delete shape;
        if (central->cutter)
            delete central->cutter;
        central->shapes.clear();

        std::ifstream fin(fileName.toStdString());

        int n;
        fin >> n;

        std::string name;
        double x0,y0,x1,y1;
        while(n--){
            fin >>name;
            if (name == "Line"){
                fin >>x0>>y0>>x1>>y1;
                central->shapes.push_back(new Line(x0, y0, x1, y1));
            }
            else {
                int m;
                fin >>m;

                QVector<QPointF> points(m);
                for(int i=0;i<m;i++){
                    double x,y;
                    fin >>x>>y;
                    points[i] = {x,y};
                }
                central->shapes.push_back(new Polygon(points));
            }
        }
        int m;
        fin >>m;

        QVector<QPointF> points(m);
        for(int i=0;i<m;i++){
            double x,y;
            fin >>x>>y;
            points[i] = {x,y};
        }
        central->cutter = new Polygon(points, Qt::blue);

        central->update();
    }
}
void MainWindow::keyPressEvent(QKeyEvent* e){
    int key = e->key();
    bool isShift = e->modifiers() & Qt::ShiftModifier;

    qreal d = 0.1;
    int dpx = 20;
    if (key == Qt::Key_Up){
        central->drawReq.yCntr += d;
    }
    if (key == Qt::Key_Down){
        central->drawReq.yCntr -= d;
    }
    if (key == Qt::Key_Left){
        central->drawReq.xCntr += d;
    }
    if (key == Qt::Key_Right){
        central->drawReq.xCntr -= d;
    }
    if (isShift && key == Qt::Key_Up){
        central->drawReq.yAskew += dpx;
    }
    if (isShift && key == Qt::Key_Down){
        central->drawReq.yAskew -= dpx;
    }
    if (isShift && key == Qt::Key_Left){
        central->drawReq.xAskew += dpx;
    }
    if (isShift && key == Qt::Key_Right){
        central->drawReq.xAskew -= dpx;
    }
    central->update();
}
void MainWindow::wheelEvent(QWheelEvent* e){
    qreal dy = e->angleDelta().y();
    qreal dZoom = 1.05;
    if (dy > 0 && central->drawReq.zoom<1) {
        central->drawReq.zoom *= dZoom;
    }
    else if (dy < 0 && central->drawReq.zoom >= 3e-4){
        central->drawReq.zoom/=dZoom;
    }
    central->update();
}
void MainWindow::mouseMoveEvent(QMouseEvent* e){
    central->myMouseMoveEvent(e);
    mousePosLabel->setText(QString("Координаты мыши (%1, %2)").arg(central->mousePos.x()).arg(central->mousePos.y()));
}
MainWindow::~MainWindow() {}

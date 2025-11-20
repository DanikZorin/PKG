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

    QAction* actionExport = new QAction("Экспортировать...");
    menuFile->addAction(actionExport);
    actionExport->setShortcut(QKeySequence("Ctrl+E"));
    connect(actionExport, &QAction::triggered, this, &MainWindow::exportImage);


    menuFile->addSeparator();

    QAction* actionClear = new QAction("Стереть");
    menuFile->addAction(actionClear);
    actionClear->setShortcut(Qt::Key_Delete);
    connect(actionClear, &QAction::triggered, [=](){
        for (Shape* shape: central->shapes){
            delete shape;
        }
        central->shapes.clear();
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

    QAction* actionToggleAskew = new QAction("Смещение сетки");
    menuView->addAction(actionToggleAskew);
    actionToggleAskew->setCheckable(true);
    actionToggleAskew->setChecked(true);
    connect(actionToggleAskew, &QAction::toggled, [=](bool state){central->drawReq.isAskew = state; central->update();});



    QMenu* menuTools = new QMenu("Инструменты");
    menuBar->addMenu(menuTools);

    QAction* actionNone = new QAction("Ничего");
    menuTools->addAction(actionNone);
    connect(actionNone, &QAction::triggered, [=](){setTool(nullptr);});

    QAction* actionDot = new QAction("Точка");
    menuTools->addAction(actionDot);
    connect(actionDot, &QAction::triggered, [=](){setTool(new Dot());});

    QAction* actionDDA = new QAction("Линия DDA");
    menuTools->addAction(actionDDA);
    connect(actionDDA, &QAction::triggered, [=](){setTool(new LineDDA());});

    QAction* actionBresenham = new QAction("Линия Брезенхем");
    menuTools->addAction(actionBresenham);
    connect(actionBresenham, &QAction::triggered, [=](){setTool(new LineBresenham());});

    QAction* actionCastla = new QAction("Линия Кастла-Питвея");
    menuTools->addAction(actionCastla);
    connect(actionCastla, &QAction::triggered, [=](){setTool(new LineCastla());});

    QAction* actionVu = new QAction("Линия Ву");
    menuTools->addAction(actionVu);
    connect(actionVu, &QAction::triggered, [=](){setTool(new LineVu());});

    menuTools->addSeparator();

    QAction* actionCircle = new QAction("Окружность Брезенхем");
    menuTools->addAction(actionCircle);
    connect(actionCircle, &QAction::triggered, [=](){setTool(new CircleBresenham());});


    QMenu* menuSettings = new QMenu("Параметры");
    menuBar->addMenu(menuSettings);

    QAction* actionColor = new QAction("Цвет пера...");
    menuSettings->addAction(actionColor);
    connect(actionColor, &QAction::triggered, [=](){

        QColor clr = QColorDialog::getColor(central->curColor);
        central->curColor = clr;
        if (central->tool && clr.isValid()){
            central->tool->setColor(clr);
        }
    });


    QMenu* submenuExpBack = new QMenu("Цвет фона экспорта");
    menuSettings->addMenu(submenuExpBack);

    QAction* actionExpBack = new QAction("Выбрать цвет...");
    submenuExpBack->addAction(actionExpBack);
    connect(actionExpBack, &QAction::triggered, [=](){

        QColor clr = QColorDialog::getColor(central->curColor);
        if (clr.isValid())
            backgroundColor = clr;
    });

    QAction* actionExpBackTransparent = new QAction("Прозрачный");
    submenuExpBack->addAction(actionExpBackTransparent);
    connect(actionExpBackTransparent, &QAction::triggered, [=](){

        QColor clr = Qt::transparent;
        if (clr.isValid())
            backgroundColor = clr;
    });

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    mousePosLabel = new QLabel("");
    statusBar->addWidget(mousePosLabel);

    curToolLabel = new QLabel("");
    statusBar->addWidget(curToolLabel);


    setWindowTitle("2D Part one");

    setMouseTracking(true);
    resize(600, 400);
}
void MainWindow::exportImage(){
    if (central->shapes.isEmpty()){
        return;
    }
    ExpReq expReq;
    expReq.start = central->shapes[0]->getMinPoint();
    expReq.end = central->shapes[0]->getMaxPoint() + QPoint(1,1);
    for (const Shape* shape: central->shapes){
        QPoint mn = shape->getMinPoint();
        QPoint mx = shape->getMaxPoint() + QPoint(1,1);
        expReq.start.setX(qMin(expReq.start.x(), mn.x()));
        expReq.start.setY(qMin(expReq.start.y(), mn.y()));
        expReq.end.setX(qMax(expReq.end.x(), mx.x()));
        expReq.end.setY(qMax(expReq.end.y(), mx.y()));
    }
    expReq.img = QImage(expReq.end.x()-expReq.start.x(),expReq.end.y()-expReq.start.y(), QImage::Format_ARGB32);
    for (int x=0;x<expReq.img.width();x++){
        for (int y=0;y<expReq.img.height();y++){
            expReq.img.setPixelColor({x,y}, backgroundColor);
        }
    }
    for (const Shape* shape: central->shapes){
        shape->exportImage(expReq);
    }


    QString fileExp = QFileDialog::getSaveFileName(0, "Экспортировать", QDir::homePath(), ".png");
    bool saveState = false;
    if (!fileExp.isEmpty()){
        saveState = expReq.img.save(fileExp+".png", "png", 100);
    }
    statusBar->showMessage("Сохранение файла...", 10000);
    if (saveState == true){
        statusBar->showMessage("Файл был сохранён успешно", 6000);
    }
    else{
        statusBar->showMessage("Файл не был сохранён", 6000);
    }
}
void MainWindow::setTool(Shape* tool){
    central->setTool(tool);
    if (tool)
        curToolLabel->setText(tool->getName());
    else
        curToolLabel->setText("");
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

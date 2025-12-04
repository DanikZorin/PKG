#include "mainwindow.h"

MainWindow::MainWindow(Projection projection, Plato plato, QWidget *parent)
    : QMainWindow(parent)
{

    area = new AreaPaint(projection, plato, this);
    setCentralWidget(area);


    move(200,100);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    //QMenu* menuAction = new QMenu("Action");
    //menuBar->addMenu(menuAction);
    //menuAction->addAction("Add Dot...");

    QMenu* menuSettings = new QMenu("Settings");
    menuBar->addMenu(menuSettings);

    QMenu* menuProjectionType = new QMenu("Type");
    menuSettings->addMenu(menuProjectionType);
    menuProjectionType->addAction("Aksonometric", area, [&](){area->setProjectionType(Aksonometric);});
    menuProjectionType->addAction("Askew", area, [&](){area->setProjectionType(Askew);});
    menuProjectionType->addAction("Central", area, [&](){area->setProjectionType(Central);});
    menuProjectionType->addAction("Perspective", area, [&](){area->setProjectionType(Perspective);});

    QMenu* menuProjectionPlato = new QMenu("Plato");
    menuSettings->addMenu(menuProjectionPlato);
    menuProjectionPlato->addAction("Frontal, Oxy", area, [&](){area->setProjectionPlato(Oxy);});
    menuProjectionPlato->addAction("Profile, Oyz", area, [&](){area->setProjectionPlato(Oyz);});
    menuProjectionPlato->addAction("Horizontal, Ozx", area, [&](){area->setProjectionPlato(Ozx);});

    QMenu* menuGridPlato = new QMenu("Grid Plato");
    menuSettings->addMenu(menuGridPlato);
    menuGridPlato->addAction("Frontal, Oxy", area, [&](){area->setGridPlato(Oxy);});
    menuGridPlato->addAction("Profile, Oyz", area, [&](){area->setGridPlato(Oyz);});
    menuGridPlato->addAction("Horizontal, Ozx", area, [&](){area->setGridPlato(Ozx);});

    //QMenu* menuHelp = new QMenu("Help");
    //menuBar->addMenu(menuHelp);
    //menuHelp->addAction("About");


    setWindowTitle("Projections");

}
void MainWindow::keyPressEvent(QKeyEvent* e){
    area->keyPressEvent(e);
}

MainWindow::~MainWindow() {}

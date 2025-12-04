#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include "areapaint.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

    AreaPaint* area;
public:
    MainWindow(Projection projection = Aksonometric, Plato plato = Oxy,QWidget *parent = nullptr);

    ~MainWindow();
    void keyPressEvent(QKeyEvent*) override;

};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <mycentral.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel* mousePosLabel;
    QLabel* curToolLabel;
    MyCentral* central;
    QStatusBar* statusBar;

    QColor backgroundColor = Qt::white;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void clearEmptyShapes();
    void exportImage();
    void setTool(Shape*);

    void keyPressEvent(QKeyEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <imageshow.h>
#include <QtCharts>
#include <algorithm>
class MainWindow : public QMainWindow
{
    Q_OBJECT

    ImageShow* showOg;
    ImageShow* showNew;

    QPushButton* buttonFile;
    QPushButton* buttonFileSaveL;
    QPushButton* buttonFileSaveR;
    QPushButton* buttonFileReplace;

    QPushButton* buttonNoise;
    QLabel* labelNoiseProb;
    QDoubleSpinBox* spinBoxNoiseProb;
    QLabel* labelNoisePower;
    QDoubleSpinBox* spinBoxNoisePower;

    QPushButton* buttonNegative;

    QPushButton* buttonLinear;
    QDoubleSpinBox* spinBoxLinearMin;
    QDoubleSpinBox* spinBoxLinearMax;

    QPushButton* buttonGray;

    QPushButton* buttonWhiteBlack;
    QDoubleSpinBox* spinBoxWhiteBlackLevel;

    QPushButton* buttonHist;
    QPushButton* buttonLocalMax;
    QPushButton* buttonLocalMin;
    QPushButton* buttonLocalMed;

    QPushButton* buttonSetRed;
    QPushButton* buttonSetGreen;
    QPushButton* buttonSetBlue;

    QPushButton* buttonMatrixGauss;
    QDoubleSpinBox* spinBoxGauss;

    QPushButton* buttonMatrixAvg;
    QDoubleSpinBox* spinBoxAvg;
private:
    void fileOpen();
    void fileSave() const;
    void fileSaveOg() const;
    void reset();
    void replace();

    void noise();
    void negative();
    void turnGray();
    void linearContrast();
    void turnWhiteBlack();
    void histEqual();
    void localMax();
    void localMin();
    void localMed();
    void setRed();
    void setGreen();
    void setBlue();
    void matrixGauss();
    void matrixAvg();

    void matrixProcess(const QVector<QVector<qreal>>& mat);

    void setVisibleHist(bool visible);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H

#include "mainwindow.h"
double getRnd(){
    return (double)std::rand()/INT16_MAX;
}
bool tryProb(double prob = .5){
    return getRnd() < prob;
}
void MainWindow::setVisibleHist(bool visible){
    showOg->setVisibleHist(visible);
    showNew->setVisibleHist(visible);
}

void MainWindow::matrixProcess(const QVector<QVector<qreal>>& mat){
    QImage img = showOg->getImage();
    QImage newImg = img;

    int size = mat.size();
    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int rSum = 0;
            int gSum = 0;
            int bSum = 0;
            int cnt = 0;
            int k = size/2;
            for (int dx=-k;dx<=k;dx++){
                for (int dy=-k;dy<=k;dy++){
                    if (x+dx >= 0 && x+dx < img.width() && y+dy >= 0 && y+dy < img.height()){
                        cnt+= mat[dx+k][dy+k];
                        int r = img.pixelColor(x+dx,y+dy).red(),g= img.pixelColor(x+dx,y+dy).green(),b= img.pixelColor(x+dx,y+dy).blue();
                        rSum += r* mat[dx+k][dy+k];
                        gSum += g* mat[dx+k][dy+k];
                        bSum += b* mat[dx+k][dy+k];
                    }
                }
            }
            if (cnt == 0){
                cnt = 1;
            }
            newImg.setPixel(x,y, qRgb(rSum/cnt, gSum/cnt, bSum/cnt));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::fileOpen(){
    QString fileName = QFileDialog::getOpenFileName(0, "", QDir::homePath(), " *.jpg ;; *.png ;; *.jpeg ");
    showOg->load(fileName);
}
void MainWindow::fileSave() const{
    if (!showNew->getImage().isNull()){
        QString fileName = QFileDialog::getSaveFileName(0, "", QDir::homePath(), " *.png ;; *.jpeg ;; *.jpg");
        showNew->getImage().save(fileName);
    }
}
void MainWindow::fileSaveOg() const{
    if (!showOg->getImage().isNull()){
        QString fileName = QFileDialog::getSaveFileName(0, "", QDir::homePath(), " *.png ;; *.jpeg ;; *.jpg");
        showOg->getImage().save(fileName);
    }
}
void MainWindow::reset(){
    showOg->reset();
    showNew->reset();
}
void MainWindow::replace(){
    showOg->setImage(showNew->getImage());
}
void MainWindow::noise(){
    const double PROB = spinBoxNoiseProb->value();
    const double noisePower = spinBoxNoisePower->value();

    std::srand(0);
    QImage img = showOg->getImage();
    QImage newImg = img;
    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            if (tryProb(PROB)){
                int newR = qMax(0., qMin(255., r+255*noisePower*(2*getRnd()-1)));
                int newG = qMax(0., qMin(255., g+255*noisePower*(2*getRnd()-1)));
                int newB = qMax(0., qMin(255., b+255*noisePower*(2*getRnd()-1)));
                newImg.setPixel(x,y, qRgb( newR, newG, newB));
            }
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::negative(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixelColor(x,y, QColor( 255-r,255-g,255-b, img.pixelColor(x,y).alpha() ));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::linearContrast(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    int Min = spinBoxLinearMin->value();
    int Max = spinBoxLinearMax->value();

    int rMin = 255;
    int rMax = 0;
    int gMin = 255;
    int gMax = 0;
    int bMin = 255;
    int bMax = 0;
    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();

            rMin = qMin(rMin, r);
            rMax = qMax(rMax, r);
            gMin = qMin(gMin, g);
            gMax = qMax(gMax, g);
            bMin = qMin(bMin, b);
            bMax = qMax(bMax, b);
        }
    }
    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb( (Max-Min)*(r-rMin)/(rMax-rMin)+Min,(Max-Min)*(g-gMin)/(gMax-gMin)+Min,(Max-Min)*(b-bMin)/(bMax-bMin)+Min ));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::turnGray(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb( (r+g+b)/3, (r+g+b)/3,(r+g+b)/3 ));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::turnWhiteBlack(){
    const int level = spinBoxWhiteBlackLevel->value() * 255;

    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            if ((r+g+b)/3 >= level){
                newImg.setPixel(x,y, qRgb( 255, 255, 255 ));
            }
            else{
                newImg.setPixel(x,y, qRgb( 0, 0, 0 ));
            }
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::histEqual(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    int rC[256]{0},gC[256]{0},bC[256]{0};
    int rS[256]{0},gS[256]{0},bS[256]{0};
    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            rC[r]++;
            gC[g]++;
            bC[b]++;
        }
    }
    rS[0] = rC[0];
    gS[0] = gC[0];
    bS[0] = bC[0];
    int rMn = rS[0]?rS[0]:255;
    int gMn = gS[0]?gS[0]:255;
    int bMn = bS[0]?bS[0]:255;
    for (int i=1;i<256;i++){
        rS[i] = rS[i-1] + rC[i];
        if (rS[i] && rS[i] < rMn){
            rMn = rS[i];
        }
        gS[i] = gS[i-1] + gC[i];
        if (gS[i] && gS[i] < gMn){
            gMn = gS[i];
        }
        bS[i] = bS[i-1] + bC[i];
        if (bS[i] && bS[i] < bMn){
            bMn = bS[i];
        }
    }

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb( 255*(rS[r]-rMn)/(img.width()*img.height()-1),
                                    255*(gS[g]-gMn)/(img.width()*img.height()-1),
                                    255*(bS[b]-bMn)/(img.width()*img.height()-1)
                                    ));
        }
    }
    showNew->setImage(newImg);
}
void MainWindow::localMax(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int rMx = 0;
            int gMx = 0;
            int bMx = 0;
            for (int dx=-1;dx<=1;dx++){
                for (int dy=-1;dy<=1;dy++){
                    if (x+dx >= 0 && x+dx < img.width() && y+dy >= 0 && y+dy < img.height()){
                        int r = img.pixelColor(x+dx,y+dy).red(),g= img.pixelColor(x+dx,y+dy).green(),b= img.pixelColor(x+dx,y+dy).blue();
                        rMx = qMax(rMx, r);
                        gMx = qMax(gMx, g);
                        bMx = qMax(bMx, b);
                    }
                }
            }
            newImg.setPixel(x,y, qRgb(rMx, gMx, bMx));

        }
    }

    showNew->setImage(newImg);
}
void MainWindow::localMin(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int rMn = 255;
            int gMn = 255;
            int bMn = 255;
            for (int dx=-1;dx<=1;dx++){
                for (int dy=-1;dy<=1;dy++){
                    if (x+dx >= 0 && x+dx < img.width() && y+dy >= 0 && y+dy < img.height()){
                        int r = img.pixelColor(x+dx,y+dy).red(),g= img.pixelColor(x+dx,y+dy).green(),b= img.pixelColor(x+dx,y+dy).blue();
                        rMn = qMin(rMn, r);
                        gMn = qMin(gMn, g);
                        bMn = qMin(bMn, b);
                    }
                }
            }
            newImg.setPixel(x,y, qRgb(rMn, gMn, bMn));

        }
    }

    showNew->setImage(newImg);
}
void MainWindow::localMed(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int rS[9];
            int gS[9];
            int bS[9];
            int cnt = 0;
            for (int dx=-1;dx<=1;dx++){
                for (int dy=-1;dy<=1;dy++){
                    if (x+dx >= 0 && x+dx < img.width() && y+dy >= 0 && y+dy < img.height()){
                        int r = img.pixelColor(x+dx,y+dy).red(),g= img.pixelColor(x+dx,y+dy).green(),b= img.pixelColor(x+dx,y+dy).blue();
                        rS[cnt] = r;
                        gS[cnt] = g;
                        bS[cnt] = b;
                        cnt++;
                    }
                }
            }
            std::sort(rS, rS+cnt);
            std::sort(gS, gS+cnt);
            std::sort(bS, bS+cnt);
            newImg.setPixel(x,y, qRgb(rS[cnt/2], gS[cnt/2], bS[cnt/2]));

        }
    }

    showNew->setImage(newImg);
}
void MainWindow::setRed(){
    QImage img = showOg->getImage();
    QImage newImg = img;


    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb(r, 0, 0));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::setGreen(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb(0, g, 0));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::setBlue(){
    QImage img = showOg->getImage();
    QImage newImg = img;

    for (int x=0;x<img.width();x++){
        for (int y=0;y<img.height();y++){
            int r = img.pixelColor(x,y).red(),g= img.pixelColor(x,y).green(),b= img.pixelColor(x,y).blue();
            newImg.setPixel(x,y, qRgb(0, 0, b));
        }
    }

    showNew->setImage(newImg);
}
void MainWindow::matrixGauss(){
    const double b = spinBoxGauss->value();

    matrixProcess( {
                   {1,b,1},
                   {b,b*b,b},
                   {1,b,1},
                   });
}
void MainWindow::matrixAvg(){
    const double a = spinBoxAvg->value();

    matrixProcess({
                   {1,1,1},
                   {1,a,1},
                   {1,1,1},
                   });

}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){


    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* menuFile = new QMenu("File");
    menuBar->addMenu(menuFile);
    QAction* actionFileOpen = new QAction("Open File...");
    menuFile->addAction(actionFileOpen);
    actionFileOpen->setShortcut(QKeySequence("Ctrl+O"));
    connect(actionFileOpen, &QAction::triggered, this, &MainWindow::fileOpen);


    QAction* actionFileSaveL = new QAction("Save File (Left)...");
    menuFile->addAction(actionFileSaveL);
    actionFileSaveL->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(actionFileSaveL, &QAction::triggered, this, &MainWindow::fileSaveOg);

    QAction* actionFileSaveR = new QAction("Save File (Right)...");
    menuFile->addAction(actionFileSaveR);
    actionFileSaveR->setShortcut(QKeySequence("Ctrl+S"));
    connect(actionFileSaveR, &QAction::triggered, this, &MainWindow::fileSave);


    menuFile->addSeparator();

    QAction* actionFileReset = new QAction("Clear");
    menuFile->addAction(actionFileReset);
    actionFileReset->setShortcut(QKeySequence("Ctrl+R"));
    connect(actionFileReset, &QAction::triggered, this, &MainWindow::reset);

    QAction* actionFileClose = new QAction("Quit");
    menuFile->addAction(actionFileClose);
    actionFileClose->setShortcut(QKeySequence("Ctrl+W"));
    connect(actionFileClose, &QAction::triggered, this, &MainWindow::close);


    QMenu* menuView = new QMenu("View");
    menuBar->addMenu(menuView);
    QAction* actionShowHist = new QAction("Show Histogramms");
    menuView->addAction(actionShowHist);
    actionShowHist->setCheckable(true);
    actionShowHist->setChecked(true);

    connect(actionShowHist, &QAction::triggered, this, &MainWindow::setVisibleHist);



    QHBoxLayout* hBoxMain = new QHBoxLayout();
    central->setLayout(hBoxMain);

    QVBoxLayout* vBox = new QVBoxLayout();
    hBoxMain->addLayout(vBox);

    QVBoxLayout* vBoxOg = new QVBoxLayout();
    hBoxMain->addLayout(vBoxOg);

    showOg = new ImageShow();
    vBoxOg->addWidget(showOg);

    QVBoxLayout* vBoxNew = new QVBoxLayout();
    hBoxMain->addLayout(vBoxNew);

    showNew = new ImageShow();
    vBoxNew->addWidget(showNew);

    buttonFile = new QPushButton("Open File");
    vBox->addWidget(buttonFile);

    connect(buttonFile, &QPushButton::clicked, this, &MainWindow::fileOpen);


    QHBoxLayout* hBoxSave = new QHBoxLayout();
    vBox->addLayout(hBoxSave);
    buttonFileSaveL = new QPushButton("Save File Left");
    hBoxSave->addWidget(buttonFileSaveL);

    connect(buttonFileSaveL, &QPushButton::clicked, this, &MainWindow::fileSaveOg);

    buttonFileSaveR = new QPushButton("Save File Right");
    hBoxSave->addWidget(buttonFileSaveR);

    connect(buttonFileSaveR, &QPushButton::clicked, this, &MainWindow::fileSave);


    buttonFileReplace = new QPushButton("Replace");
    vBox->addWidget(buttonFileReplace);

    connect(buttonFileReplace, &QPushButton::clicked, this, &MainWindow::replace);

    buttonNoise = new QPushButton("Noise");
    vBox->addWidget(buttonNoise);

    QHBoxLayout* hBoxNoiseSettings = new QHBoxLayout();
    vBox->addLayout(hBoxNoiseSettings);

    labelNoiseProb = new QLabel("Частота");
    hBoxNoiseSettings->addWidget(labelNoiseProb);

    spinBoxNoiseProb = new QDoubleSpinBox();
    hBoxNoiseSettings->addWidget(spinBoxNoiseProb);
    spinBoxNoiseProb->setDecimals(2);
    spinBoxNoiseProb->setMinimum(0);
    spinBoxNoiseProb->setMaximum(1);
    spinBoxNoiseProb->setSingleStep(0.01);
    spinBoxNoiseProb->setValue(0.2);
    spinBoxNoiseProb->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxNoiseProb->setFixedWidth(50);


    labelNoisePower = new QLabel("Искажение");
    hBoxNoiseSettings->addWidget(labelNoisePower);

    spinBoxNoisePower = new QDoubleSpinBox();
    hBoxNoiseSettings->addWidget(spinBoxNoisePower);
    spinBoxNoisePower->setDecimals(2);
    spinBoxNoisePower->setMinimum(0);
    spinBoxNoisePower->setMaximum(1);
    spinBoxNoisePower->setSingleStep(0.01);
    spinBoxNoisePower->setValue(0.3);
    spinBoxNoisePower->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxNoisePower->setFixedWidth(50);

    connect(buttonNoise, &QPushButton::clicked, this, &MainWindow::noise);

    buttonNegative = new QPushButton("Negative");
    vBox->addWidget(buttonNegative);

    connect(buttonNegative, &QPushButton::clicked, this, &MainWindow::negative);

    QHBoxLayout* hBoxLinear = new QHBoxLayout();
    vBox->addLayout(hBoxLinear);

    buttonLinear = new QPushButton("Линейное контрастирование");
    hBoxLinear->addWidget(buttonLinear);

    spinBoxLinearMin = new QDoubleSpinBox();
    hBoxLinear->addWidget(spinBoxLinearMin);
    spinBoxLinearMin->setDecimals(0);
    spinBoxLinearMin->setMinimum(0);
    spinBoxLinearMin->setMaximum(255);
    spinBoxLinearMin->setSingleStep(1);
    spinBoxLinearMin->setValue(0);
    spinBoxLinearMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxLinearMin->setFixedWidth(50);

    spinBoxLinearMax = new QDoubleSpinBox();
    hBoxLinear->addWidget(spinBoxLinearMax);
    spinBoxLinearMax->setDecimals(0);
    spinBoxLinearMax->setMinimum(0);
    spinBoxLinearMax->setMaximum(255);
    spinBoxLinearMax->setSingleStep(1);
    spinBoxLinearMax->setValue(255);
    spinBoxLinearMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxLinearMax->setFixedWidth(50);

    connect(buttonLinear, &QPushButton::clicked, this, &MainWindow::linearContrast);

    buttonGray = new QPushButton("Turn gray");
    vBox->addWidget(buttonGray);

    connect(buttonGray, &QPushButton::clicked, this, &MainWindow::turnGray);

    QHBoxLayout* hBoxWhiteBlack = new QHBoxLayout();
    vBox->addLayout(hBoxWhiteBlack);

    buttonWhiteBlack = new QPushButton("Turn WhiteBlack");
    hBoxWhiteBlack->addWidget(buttonWhiteBlack);

    spinBoxWhiteBlackLevel = new QDoubleSpinBox();
    hBoxWhiteBlack->addWidget(spinBoxWhiteBlackLevel);
    spinBoxWhiteBlackLevel->setDecimals(2);
    spinBoxWhiteBlackLevel->setMinimum(0);
    spinBoxWhiteBlackLevel->setMaximum(1);
    spinBoxWhiteBlackLevel->setSingleStep(0.01);
    spinBoxWhiteBlackLevel->setValue(0.5);
    spinBoxWhiteBlackLevel->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxWhiteBlackLevel->setFixedWidth(50);


    connect(buttonWhiteBlack, &QPushButton::clicked, this, &MainWindow::turnWhiteBlack);

    buttonHist = new QPushButton("Выравнивание гистограммы");
    vBox->addWidget(buttonHist);
    connect(buttonHist, &QPushButton::clicked, this, &MainWindow::histEqual);


    buttonLocalMax = new QPushButton("Локальный максимум");
    vBox->addWidget(buttonLocalMax);
    connect(buttonLocalMax, &QPushButton::clicked, this, &MainWindow::localMax);


    buttonLocalMin = new QPushButton("Локальный минимум");
    vBox->addWidget(buttonLocalMin);
    connect(buttonLocalMin, &QPushButton::clicked, this, &MainWindow::localMin);


    buttonLocalMed = new QPushButton("Локальный медианный");
    vBox->addWidget(buttonLocalMed);
    connect(buttonLocalMed, &QPushButton::clicked, this, &MainWindow::localMed);

    QHBoxLayout* hBoxButtonsRGB = new QHBoxLayout();
    vBox->addLayout(hBoxButtonsRGB);

    buttonSetRed = new QPushButton("RED");
    hBoxButtonsRGB->addWidget(buttonSetRed);
    connect(buttonSetRed, &QPushButton::clicked, this, &MainWindow::setRed);

    buttonSetGreen = new QPushButton("GREEN");
    hBoxButtonsRGB->addWidget(buttonSetGreen);
    connect(buttonSetGreen, &QPushButton::clicked, this, &MainWindow::setGreen);

    buttonSetBlue = new QPushButton("BLUE");
    hBoxButtonsRGB->addWidget(buttonSetBlue);
    connect(buttonSetBlue, &QPushButton::clicked, this, &MainWindow::setBlue);


    QHBoxLayout* hBoxGauss = new QHBoxLayout();
    vBox->addLayout(hBoxGauss);

    buttonMatrixGauss = new QPushButton("Фильтр Гаусса");
    hBoxGauss->addWidget(buttonMatrixGauss);

    spinBoxGauss = new QDoubleSpinBox();
    hBoxGauss->addWidget(spinBoxGauss);

    spinBoxGauss->setDecimals(0);
    //spinBoxGauss->setMaximum(4);
    spinBoxGauss->setMinimum(0);
    spinBoxGauss->setSingleStep(1);
    spinBoxGauss->setValue(2);
    spinBoxGauss->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxGauss->setFixedWidth(50);

    connect(buttonMatrixGauss, &QPushButton::clicked, this, &MainWindow::matrixGauss);


    QHBoxLayout* hBoxAvg = new QHBoxLayout();
    vBox->addLayout(hBoxAvg);


    buttonMatrixAvg = new QPushButton("Фильтр усредняющий");
    hBoxAvg->addWidget(buttonMatrixAvg);

    spinBoxAvg = new QDoubleSpinBox();
    hBoxAvg->addWidget(spinBoxAvg);

    spinBoxAvg->setDecimals(0);
    spinBoxAvg->setMaximum(10);
    spinBoxAvg->setMinimum(0);
    spinBoxAvg->setSingleStep(1);
    spinBoxAvg->setValue(1);
    spinBoxAvg->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBoxAvg->setFixedWidth(50);

    connect(buttonMatrixAvg, &QPushButton::clicked, this, &MainWindow::matrixAvg);
}


MainWindow::~MainWindow() {}

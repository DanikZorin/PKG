#include "imageshow.h"

ImageShow::ImageShow(QWidget* parent):QWidget(parent) {
    QVBoxLayout* vBox = new QVBoxLayout(this);
    setLayout(vBox);

    imageBlock = new ImageBlock();
    vBox->addWidget(imageBlock);

    rBar = new QBarSet("");
    rBar->setPen(QPen(Qt::red));
    gBar = new QBarSet("");
    gBar->setPen(QPen(Qt::green));
    bBar = new QBarSet("");
    bBar->setPen(QPen(Qt::blue));
    rSer = new QBarSeries();
    gSer = new QBarSeries();
    bSer = new QBarSeries();
    rChart = new QChart();
//    rChart->setTitle("Red");
    gChart = new QChart();
//    gChart->setTitle("Green");
    bChart = new QChart();
//    bChart->setTitle("Blue");
    rAxisX = new QValueAxis();
    rAxisY = new QValueAxis();
    gAxisX = new QValueAxis();
    gAxisY = new QValueAxis();
    bAxisX = new QValueAxis();
    bAxisY = new QValueAxis();
    rChartView = new QChartView();
    gChartView = new QChartView();
    bChartView = new QChartView();
    QBarSet* bars[] = {rBar, gBar, bBar};
    QBarSeries* sers[] = {rSer, gSer, bSer};
    QChart* charts[] = {rChart, gChart, bChart};
    QValueAxis* axisXs[] = {rAxisX, gAxisX, bAxisX};
    QValueAxis* axisYs[] = {rAxisY, gAxisY, bAxisY};
    QChartView* chartViews[] = {rChartView, gChartView, bChartView};
    for (int i=0;i<3;i++){
        QBarSet* bar = bars[i];
        QBarSeries* ser = sers[i];
        QChart* chart = charts[i];
        QValueAxis* axisX = axisXs[i];
        QValueAxis* axisY = axisYs[i];
        QChartView* chartView = chartViews[i];

        for (int i=0;i<=255;i++){
            (*bar)<<0;
        }
        ser->append(bar);
        chart->addSeries(ser);

        axisX->setRange(0,255);
        axisX->setVisible(false);
        chart->addAxis(axisX, Qt::AlignBottom);
        ser->attachAxis(axisX);

        axisY->setRange(0,100);
        axisY->setVisible(false);
        chart->addAxis(axisY, Qt::AlignLeft);
        ser->attachAxis(axisY);

        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->setVisible(true);
        chart->setPlotAreaBackgroundVisible(true);
        chart->legend()->hide();
        chart->setMargins(QMargins(3,0,3,0));
        chart->setBackgroundRoundness(5);

        chartView->setChart(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setFixedHeight(90);
        chartView->setVisible(true);

        vBox->addWidget(chartView);
    }

}
void ImageShow::setImage(const QImage& img){
    imageBlock->setImage(img);
    updateChart();
}
QImage ImageShow::getImage() const{
    return imageBlock->getImage();
}
void ImageShow::load(const QString& fileName){
    imageBlock->load(fileName);
    updateChart();
}
void ImageShow::reset(){
    imageBlock->reset();
    updateChart();
}
void ImageShow::hideHist(){
    rChartView->setVisible(false);
    gChartView->setVisible(false);
    bChartView->setVisible(false);
}
void ImageShow::showHist(){
    rChartView->setVisible(true);
    gChartView->setVisible(true);
    bChartView->setVisible(true);
}
void ImageShow::setVisibleHist(bool visible){
    if (visible){
        showHist();
    }
    else{
        hideHist();
    }
}

void ImageShow::updateChart(){
    QImage image = imageBlock->getImage();

    int rC[256]{0},gC[256]{0},bC[256]{0};
    int rMx = 0, gMx = 0, bMx = 0;
    for (int x=0;x<image.width();x++){
        for (int y=0;y<image.height();y++){
            int r = image.pixelColor(x,y).red(),g= image.pixelColor(x,y).green(),b= image.pixelColor(x,y).blue();
            rC[r]++;
            gC[g]++;
            bC[b]++;
        }
    }

    for (int i=0;i<=255;i++){
        rMx = qMax(rMx, rC[i]);
        gMx = qMax(gMx, gC[i]);
        bMx = qMax(bMx, bC[i]);
    }

    for (int i=0;i<=255;i++){
        if (rMx == 0)
            rBar->replace(i, 0);
        else
            rBar->replace(i, 100.*rC[i]/rMx);

        if (gMx == 0)
            gBar->replace(i, 0);
        else
            gBar->replace(i, 100.*gC[i]/gMx);

        if (bMx == 0)
            bBar->replace(i, 0);
        else
            bBar->replace(i, 100.*bC[i]/bMx);
    }
}

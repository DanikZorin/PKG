#ifndef IMAGESHOW_H
#define IMAGESHOW_H

#include <QtWidgets>
#include <QtCharts>
#include <imageblock.h>
class ImageShow: public QWidget{
    Q_OBJECT
private:
    QBarSet* rBar;
    QBarSet* gBar;
    QBarSet* bBar;
    QBarSeries* rSer;
    QBarSeries* gSer;
    QBarSeries* bSer;
    QChart* rChart;
    QChart* gChart;
    QChart* bChart;
    QValueAxis *rAxisX;
    QValueAxis *rAxisY;
    QValueAxis *gAxisX;
    QValueAxis *gAxisY;
    QValueAxis *bAxisX;
    QValueAxis *bAxisY;
    QChartView* rChartView;
    QChartView* gChartView;
    QChartView* bChartView;

    ImageBlock* imageBlock;

    void updateChart();
public:

    ImageShow(QWidget* parent = 0);

    void setImage(const QImage&);
    QImage getImage() const;
    void load(const QString& fileName);
    void reset();

    void hideHist();
    void showHist();
    void setVisibleHist(bool visible);
};

#endif // IMAGESHOW_H

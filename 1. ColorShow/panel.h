#ifndef PANEL_H
#define PANEL_H

#include <QSlider>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QBoxLayout>
#include "colormodel.h"
#include <QStyleOption>
#include <QPainter>
class Panel: public QWidget{

    Q_OBJECT

    QSlider* slider[5];
    QDoubleSpinBox* lines[5];
    ColorModel* colorModel;

    void paintEvent(QPaintEvent*) override;
public:
    Panel(ColorModel* colorModel, QWidget* parent = nullptr);
    void setColor(const RGB& newColor);
    void emitNewColor();


signals:
    void colorChange(RGB);
};


#endif // PANEL_H

#include "panel.h"

Panel::Panel(ColorModel* colorModel_, QWidget* parent): QWidget(parent), colorModel(colorModel_){

    QVBoxLayout* vBox = new QVBoxLayout(this);
    setLayout(vBox);
    vBox->setAlignment(Qt::AlignTop);
    QLabel* label = new QLabel(colorModel->getName());
    label->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    vBox->addWidget(label);
    for (int i=0;i<colorModel->getChannelCnt();i++){
        QHBoxLayout* hBox = new QHBoxLayout();
        vBox->addLayout(hBox);
        QLabel* channelName = new QLabel(colorModel->getName()[i]);
        channelName->setMaximumWidth(24);
        channelName->setObjectName("channelName");
        channelName->setAlignment(Qt::AlignCenter);
        hBox->addWidget(channelName);

        lines[i] = new QDoubleSpinBox();
        lines[i]->setMinimumWidth(70);
        lines[i]->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lines[i]->setAlignment(Qt::AlignCenter);
        hBox->addWidget(lines[i]);
        QPair<int,int> minMax = colorModel->getInterval(i);
        lines[i]->setMinimum(minMax.first);
        lines[i]->setMaximum(minMax.second);
        lines[i]->setSingleStep(minMax.second==255?1:0.01);
        lines[i]->setDecimals(minMax.second==255?0:2);

        slider[i] = new QSlider(Qt::Orientation::Horizontal);
        vBox->addWidget(slider[i]);


        connect(slider[i], &QSlider::valueChanged, lines[i], [i, this](double val){
            lines[i]->blockSignals(true);
            lines[i]->setValue(val/99*lines[i]->maximum());
            lines[i]->blockSignals(false);
            this->emitNewColor();
        });

        connect(lines[i], &QDoubleSpinBox::valueChanged, this, [i, this](){
            slider[i]->blockSignals(true);
            slider[i]->setValue(lines[i]->value()/lines[i]->maximum()*99);
            slider[i]->blockSignals(false);
            this->emitNewColor();
        });
    }

    vBox->setContentsMargins(QMargins(0,0,0,53*(4-colorModel->getChannelCnt())));
}
void Panel::setColor(const RGB& rgb){
    colorModel->setColor(rgb);

    qreal* clr = colorModel->getVals();
    for (int i=0;i<colorModel->getChannelCnt();i++){
        lines[i]->blockSignals(true);
        lines[i]->setValue(clr[i]);
        lines[i]->blockSignals(false);
        slider[i]->blockSignals(true);
        slider[i]->setValue(lines[i]->value()/lines[i]->maximum()*99);
        slider[i]->blockSignals(false);
    }
    delete clr;
}
void Panel::emitNewColor(){
    qreal clr[5];
    for (int i=0;i<colorModel->getChannelCnt();i++){
        clr[i] = lines[i]->value();
    }
    emit colorChange(colorModel->getRGB(clr));
};
void Panel::paintEvent(QPaintEvent* e){
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

#include "imageblock.h"

ImageBlock::ImageBlock(QWidget *parent): QWidget{parent}{
    setMinimumSize(200, 200);
    //setMaximumSize(300, 300);
    setAutoFillBackground(true);
}
void ImageBlock::setImage(const QImage& img){
    image = img;
    update();
}
QImage ImageBlock::getImage() const{
    return image;
}
void ImageBlock::load(const QString& fileName){
    image.load(fileName);
    update();
}
void ImageBlock::reset(){
    image = QImage();
    update();
}


void ImageBlock::paintEvent(QPaintEvent*){
    if (image.isNull()){
        return;
    }
    qreal kX = (qreal)width() / image.width();
    qreal kY = (qreal)height() / image.height();
    qreal k = qMin(kX, kY);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawImage(0,0, image.scaled(image.width()*k,image.height()*k));

}


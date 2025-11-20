#ifndef COLORMODEL_H
#define COLORMODEL_H

#include <QWidget>
class RGB;
class CMYK;
class HLS;
class ColorModel{
public:
    virtual int getChannelCnt() const = 0;
    virtual QString getName() const = 0;
    virtual QPair<int, int> getInterval(int ind) const = 0;
    virtual RGB getRGB(qreal*) const = 0;
    virtual void setColor(const RGB&) = 0;
    virtual qreal* getVals() const = 0;
};

class RGB: public ColorModel{
public:
    int r,g,b;

    RGB(int r=0, int g=0, int b=0);
    RGB(const QColor);
    RGB(const RGB& rgb);
    RGB(const HLS& hls);
    RGB(const CMYK& cmyk);
    QColor getQColor() const;

    int getChannelCnt() const override;
    QString getName() const override;
    QPair<int,int> getInterval(int ind) const override;
    RGB getRGB(qreal* rgb) const override;
    void setColor(const RGB&) override;
    qreal* getVals() const override;
};
class HLS: public ColorModel{
public:
    qreal h,l,s;

    HLS(qreal h=0, qreal l=0, qreal s=0);
    HLS(const RGB& rgb);
    int getChannelCnt() const override;
    QString getName() const override;
    QPair<int,int> getInterval(int ind) const override;
    RGB getRGB(qreal* hls) const override;
    void setColor(const RGB&) override;
    qreal* getVals() const override;
};
class CMYK: public ColorModel{
public:
    qreal c,m,y,k;

    CMYK(qreal c=0, qreal m=0, qreal y=0, qreal k=0);
    CMYK(const RGB& rgb);
    int getChannelCnt() const override;
    QString getName() const override;
    QPair<int,int> getInterval(int ind) const override;
    RGB getRGB(qreal* cmyk) const override;
    void setColor(const RGB&) override;
    qreal* getVals() const override;
};



#endif // COLORMODEL_H

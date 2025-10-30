#include "colormodel.h"
RGB::RGB(int r, int g, int b):r(r),g(g),b(b){

}
RGB::RGB(const QColor clr):RGB(clr.red(), clr.green(), clr.blue()){

}

RGB::RGB(const RGB& rgb){
    r = rgb.r;
    g = rgb.g;
    b = rgb.b;
}

RGB::RGB(const HLS& hls){
    qreal l = hls.l;
    qreal s = hls.s;
    qreal q = (l<0.5? l*(1+s) : l + s - l*s );
    qreal p = 2*l-q;
    qreal h_ = hls.h / 360.;

    qreal t[3] = { h_+1/3., h_, h_-1/3. };
    qreal clr[3];
    for (int i=0;i<3;i++){
        if (t[i] < 0)
            t[i] += 1;
        if (t[i] > 1)
            t[i] -= 1;

        if (t[i] < 1/6.){
            clr[i] = p + (q-p)*t[i]*6;
        }
        else if (t[i] < 1/2.){
            clr[i] = q;
        }
        else if (t[i] < 2/3.){
            clr[i] = p + (q-p)*(2/3.-t[i])*6;
        }
        else{
            clr[i] = p;
        }
    }
    r = clr[0] * 255;
    g = clr[1] * 255;
    b = clr[2] * 255;
}

RGB::RGB(const CMYK& cmyk){
    r = 255 * (1-cmyk.c) * (1-cmyk.k);
    g = 255 * (1-cmyk.m) * (1-cmyk.k);
    b = 255 * (1-cmyk.y) * (1-cmyk.k);
}
QColor RGB::getQColor() const{
    return QColor(r,g,b);
}

int RGB::getChannelCnt() const{
    return 3;
}
QString RGB::getName() const{
    return "RGB";
}
QPair<int,int> RGB::getInterval(int ind) const{
    return {0, 255};
}
RGB RGB::getRGB(qreal* rgb) const{
    return RGB(rgb[0],rgb[1],rgb[2]);
}
void RGB::setColor(const RGB& rgb){
    RGB res(rgb);

    r = res.r;
    g = res.g;
    b = res.b;
}
qreal* RGB::getVals() const{
    qreal* clr = new qreal[5];
    clr[0] = r;
    clr[1] = g;
    clr[2] = b;
    return clr;
}


HLS::HLS(qreal h, qreal l, qreal s): h(h), l(l), s(s){

}
HLS::HLS(const RGB& rgb){
    qreal r = rgb.r/255.;
    qreal g = rgb.g/255.;
    qreal b = rgb.b/255.;

    qreal mx = std::max(std::max(r,g),b);
    qreal mn = std::min(std::min(r,g),b);

    if (mx == mn){
        h = 0;
    }
    else if (mx == r && g>=b){
        h = 60 * (g-b) / (mx-mn);
    }
    else if (mx == r && g<b){
        h = 60 * (g-b) / (mx-mn) + 360;
    }
    else if (mx == g){
        h = 60 * (b-r) / (mx-mn) + 120;
    }
    else if (mx == b){
        h = 60 * (r-g) / (mx-mn) + 240;
    }

    l = (mx+mn)/2;

    if (l==0 || mx == mn){
        s = 0;
    }
    else if (l<=1/2.){
        s = (mx-mn) / (mx+mn);
    }
    else if (l<1){
        s = (mx-mn) / (2-mx-mn);
    }
}
int HLS::getChannelCnt() const{
    return 3;
}
QString HLS::getName() const{
    return "HLS";
}
QPair<int,int> HLS::getInterval(int ind) const{
    if (ind == 0){
        return {0, 360};
    }
    else{
        return {0,1};
    }
}
RGB HLS::getRGB(qreal* hls) const{
    return HLS(hls[0],hls[1],hls[2]);
}
void HLS::setColor(const RGB& rgb){
    HLS res(rgb);

    h = res.h;
    l = res.l;
    s = res.s;
}
qreal* HLS::getVals() const{
    qreal* clr = new qreal[5];
    clr[0] = h;
    clr[1] = l;
    clr[2] = s;
    return clr;
}


CMYK::CMYK(qreal c, qreal m, qreal y, qreal k): c(c), m(m), y(y), k(k){

}
CMYK::CMYK(const RGB& rgb){
    k = 1 - std::max(std::max(rgb.r, rgb.g), rgb.b)/255.;
    if (k == 1){
        c = 1;
        m = 1;
        y = 1;
    }
    else{
        c = (1-rgb.r/255.-k)/(1-k);
        m = (1-rgb.g/255.-k)/(1-k);
        y = (1-rgb.b/255.-k)/(1-k);
    }
}
int CMYK::getChannelCnt() const{
    return 4;
}
QString CMYK::getName() const{
    return "CMYK";
}
QPair<int,int> CMYK::getInterval(int ind) const{
    return {0,1};
}
RGB CMYK::getRGB(qreal* cmyk) const{
    return CMYK(cmyk[0],cmyk[1],cmyk[2],cmyk[3]);
}
void CMYK::setColor(const RGB& rgb){
    CMYK res(rgb);

    c = res.c;
    m = res.m;
    y = res.y;
    k = res.k;
}
qreal* CMYK::getVals() const{
    qreal* clr = new qreal[5];
    clr[0] = c;
    clr[1] = m;
    clr[2] = y;
    clr[3] = k;
    return clr;
}


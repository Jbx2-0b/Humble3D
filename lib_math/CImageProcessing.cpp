#include "CImageProcessing.h"


// value noise, and its analytical derivatives
//QVector3D CImageProcessing::noised(const QImage& image, const QVector2D& v)
//{
//    QVector2D p = floor(v);
//    QVector2D f = fract(v);
//    QVector2D u = f*f*(3.0-2.0*f);
//    float a = getPixel(image,(p+vec2(0.5,0.5))/256.0,-100.0).x();
//    float b = getPixel(image,(p+vec2(1.5,0.5))/256.0,-100.0).x();
//    float c = getPixel(image,(p+vec2(0.5,1.5))/256.0,-100.0).x();
//    float d = getPixel(image,(p+vec2(1.5,1.5))/256.0,-100.0).x();
//    return QVector3D(a+(b-a)*u.x()+(c-a)*u.y()+(a-b-c+d)*u.x()*u.y(),
//                6.0*f*(1.0-f)*(QVector2D((b-a,c-a)+(a-b-c+d)*u.y(), (b-a,c-a)+(a-b-c+d)*u.x())));
//}

//!
QVector3D CImageProcessing::getPixel(const QImage& img, real x, real y)
{
    real dW = x / (real) img.width();
    real dH = y / (real) img.height();

    QColor color = QColor(img.pixel((int)dW, (int)dH));
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

QImage CImageProcessing::changeBrightness(const QImage& image, int brightness)
{
    if(brightness == 0) // no change
        return image;
    return changeImage< changeBrightness >(image, brightness);
}

QImage CImageProcessing::changeContrast(const QImage& image, int contrast)
{
    if(contrast == 100) // no change
        return image;
    return changeImage< changeContrast >(image, contrast);
}

QImage CImageProcessing::changeGamma(const QImage& image, int gamma)
{
    if(gamma == 100) // no change
        return image;
    return changeImage< changeGamma >(image, gamma);
}

QPixmap CImageProcessing::changeBrightness(const QPixmap& pixmap, int brightness)
{
    QImage image = pixmap.toImage();
    return QPixmap::fromImage(changeBrightness(image, brightness));
}

QPixmap CImageProcessing::changeContrast(const QPixmap& pixmap, int contrast)
{
    QImage image = pixmap.toImage();
    return QPixmap::fromImage(changeContrast(image, contrast));
}

QPixmap CImageProcessing::changeGamma(const QPixmap& pixmap, int gamma)
{
    QImage image = pixmap.toImage();
    return QPixmap::fromImage(changeBrightness(image, gamma));
}

int CImageProcessing::changeBrightness(int value, int brightness)
{
    return kClamp(value + brightness * 255 / 100, 0, 255);
}

int CImageProcessing::changeContrast(int value, int contrast)
{
    return kClamp(((value - 127) * contrast / 100) + 127, 0, 255);
}

int CImageProcessing::changeGamma(int value, int gamma)
{
    return kClamp(int(pow(value / 255.0, 100.0 / gamma) * 255), 0, 255);
}

int CImageProcessing::changeUsingTable(int value, const int table[])
{
    return table[value];
}

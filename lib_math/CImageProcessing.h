#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

// Lib
#include "lib_math_global.h"

// Qt
#include <QImage>
#include <QPixmap>

// Lib
#include "CMath.h"

class LIB_MATHSHARED_EXPORT CImageProcessing
{
public:

    // value noise, and its analytical derivatives
    //static QVector3D noised(const QImage& image, const QVector2D& v);

    //!
    static QVector3D getPixel(const QImage& img, real x, real y);

    // Brightness is multiplied by 100 in order to avoid floating point numbers
    static QImage changeBrightness(const QImage& image, int brightness);

    // Contrast is multiplied by 100 in order to avoid floating point numbers
    static QImage changeContrast(const QImage& image, int contrast);

    // Gamma is multiplied by 100 in order to avoid floating point numbers
    static QImage changeGamma(const QImage& image, int gamma);

    static QPixmap changeBrightness(const QPixmap& pixmap, int brightness);

    static QPixmap changeContrast(const QPixmap& pixmap, int contrast);

    static QPixmap changeGamma(const QPixmap& pixmap, int gamma);

private:

    template< int operation(int, int) >
    static QImage changeImage(const QImage& image, int value)
    {
        QImage im = image;
        im.detach();
        if(im.colorCount() == 0) /* truecolor */
        {
            if(im.format() != QImage::Format_RGB32) /* just in case */
                im = im.convertToFormat(QImage::Format_RGB32);
            int table[256];
            for(int i = 0;
                i < 256;
                ++i)
                table[i] = operation(i, value);
            if(im.hasAlphaChannel())
            {
                for(int y = 0;
                    y < im.height();
                    ++y)
                {
                    QRgb* line = reinterpret_cast< QRgb* >(im.scanLine(y));
                    for(int x = 0;
                        x < im.width();
                        ++x)
                        line[x] = qRgba(changeUsingTable(qRed(line[x]), table),
                                        changeUsingTable(qGreen(line[x]), table),
                                        changeUsingTable(qBlue(line[x]), table),
                                        changeUsingTable(qAlpha(line[x]), table));
                }
            }
            else
            {
                for(int y = 0;
                    y < im.height();
                    ++y)
                {
                    QRgb* line = reinterpret_cast< QRgb* >(im.scanLine(y));

                    for(int x = 0; x < im.width(); ++x)
                        line[x] = qRgb(changeUsingTable(qRed(line[x]), table),
                                       changeUsingTable(qGreen(line[x]), table),
                                       changeUsingTable(qBlue(line[x]), table));
                }
            }
        }
        else
        {
            QVector<QRgb> colors = im.colorTable();
            for(int i = 0;
                i < im.colorCount();
                ++i)
                colors[i] = qRgb(operation(qRed(colors[i]), value),
                                 operation(qGreen(colors[i]), value),
                                 operation(qBlue(colors[i]), value));
        }
        return im;
    }

    template<class T>
    static const T& kClamp(const T& x, const T& low, const T& high)
    {
        if (x < low) return low;
        else if (high < x) return high;
        else return x;
    }

    static int changeBrightness(int value, int brightness);

    static int changeContrast(int value, int contrast);

    static int changeGamma(int value, int gamma);

    static int changeUsingTable(int value, const int table[]);
};

#endif // IMAGEPROCESSING_H

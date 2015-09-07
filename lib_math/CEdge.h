#ifndef CEDGE_H
#define CEDGE_H

// Qt
#include <QVector4D>


class CEdge
{

public:

    QVector4D fColor1, fColor2;
    int fX1, fY1, fX2, fY2;
    double fDistance1, fDistance2;

    CEdge(const QVector4D& aColor1, double zValue1, int aX1, int aY1, const QVector4D& aColor2, double zValue2, int aX2, int aY2)
    {
        if(aY1 < aY2)
        {
            fColor1 = aColor1;
            fDistance1 = zValue1;
            fX1 = aX1;
            fY1 = aY1;
            fColor2 = aColor2;
            fDistance2 = zValue2;
            fX2 = aX2;
            fY2 = aY2;
        }
        else
        {
            fColor1 = aColor2;
            fDistance1 = zValue2;
            fX1 = aX2;
            fY1 = aY2;
            fColor2 = aColor1;
            fDistance2 = zValue1;
            fX2 = aX1;
            fY2 = aY1;
        }
    }
};

#endif // CEDGE_H

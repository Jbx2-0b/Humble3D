#ifndef CLINE_H
#define CLINE_H

// Qt
#include <QVector2D>


class CLine
{
public:
    static bool intersects(QVector2D aLineStart1, QVector2D aLineEnd1, QVector2D aLineStart2, QVector2D aLineEnd2)
    {
        QVector2D b = aLineEnd1 - aLineStart1;
        QVector2D d = aLineEnd2 - aLineStart2;


        double bDotDPerp = b.x() * d.y() - b.y() * d.x();

        // if b dot d == 0, it means the lines are parallel so have infinite intersection points
        if (bDotDPerp == 0)
            return false;

        QVector2D c = aLineStart2 - aLineStart1;
        double t = (c.x() * d.y() - c.y() * d.x()) / bDotDPerp;
        if (t < 0 || t > 1)
            return false;

        double u = (c.x() * b.y() - c.y() * b.x()) / bDotDPerp;
        if (u < 0 || u > 1)
            return false;

        //Vector2d intersection = Vector2d.plus(a1, Vector2d.mul(b,t));

        return true;
    }
};


#endif // CLINE_H

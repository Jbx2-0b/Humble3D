#ifndef CSPAN_H
#define CSPAN_H

// Qt
#include <QVector4D>


class CSpan
{
public:
	QVector4D fColor1, fColor2;
	int fX1, fX2;
	double fDistance1, fDistance2;

	CSpan(const QVector4D& aColor1, double dZValue1, int aX1, const QVector4D& aColor2, double dZValue2, int aX2)
	{
		if(aX1 < aX2)
		{
			fColor1 = aColor1;
			fDistance1 = dZValue1;
			fX1 = aX1;
			fColor2 = aColor2;
			fDistance2 = dZValue2;
			fX2 = aX2;
		}
		else
		{
			fColor1 = aColor2;
			fDistance1 = dZValue2;
			fX1 = aX2;
			fColor2 = aColor1;
			fDistance2 = dZValue1;
			fX2 = aX1;
		}
	}
};

#endif // CSPAN_H

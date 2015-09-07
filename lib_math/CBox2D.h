#ifndef CBOX2D_H
#define CBOX2D_H

// Qt
#include <QVector2D>

// Foundations
#include "CLine.h"

class CBox2D
{
public:
	QVector2D fUpLeft, fUpRight, fDownLeft, fDownRight;


    CBox2D(real aX0, real aY0, real aX1, real aY1)
	{
        real lMinX = qMin(aX0, aX1);
        real lMinY = qMin(aY0, aY1);
        real lMaxX = qMax(aX0, aX1);
        real lMaxY = qMax(aY0, aY1);

		fUpLeft 	= QVector2D(lMinX, lMinY);
		fUpRight 	= QVector2D(lMaxX, lMinY);
		fDownLeft	= QVector2D(lMinX, lMaxY);
		fDownRight	= QVector2D(lMaxX, lMaxY);
	}

	QVector2D getUpLeft() 		const	{ return fUpLeft; }
	QVector2D getUpRight() 		const	{ return fUpRight; }
	QVector2D getDownLeft()		const	{ return fDownLeft; }
	QVector2D getDownRight() 	const	{ return fDownRight; }
	QVector2D getMin() 			const	{ return fUpLeft; }
	QVector2D getMax() 			const	{ return fDownRight; }

	bool contains(QVector2D aPt) const
	{
		if (aPt.x() >= getMin().x() && aPt.x() <= getMax().x() &&
			aPt.y() >= getMin().y() && aPt.y() <= getMax().y())
		{
			return true;
		}	

		return false;
	}


    static bool intersect(const CBox2D& aBbox1, const CBox2D& aBbox2)
	{
		bool lB1iB2 = 
			aBbox1.contains(aBbox2.getUpLeft()) 	|| 
			aBbox1.contains(aBbox2.getUpRight()) 	||
			aBbox1.contains(aBbox2.getDownLeft()) 	||
			aBbox1.contains(aBbox2.getDownRight());

		bool lB2iB1 = 
			aBbox2.contains(aBbox1.getUpLeft()) 	|| 
			aBbox2.contains(aBbox1.getUpRight()) 	||
			aBbox2.contains(aBbox1.getDownLeft()) 	||
			aBbox2.contains(aBbox1.getDownRight());

		return lB1iB2 || lB2iB1; 

	}

	bool intersect(const QVector2D& aLineStart, const QVector2D& aLineEnd)
	{
		return 	contains(aLineStart) ||
				contains(aLineEnd)   ||
				CLine::intersects(aLineStart, aLineEnd, getUpLeft(), 	getUpRight()) 	||
				CLine::intersects(aLineStart, aLineEnd, getDownLeft(), 	getDownRight())	||
				CLine::intersects(aLineStart, aLineEnd, getDownLeft(), 	getUpLeft()) 	||
				CLine::intersects(aLineStart, aLineEnd, getDownRight(),	getUpRight());

	}
};

#endif // CBOX2D_H

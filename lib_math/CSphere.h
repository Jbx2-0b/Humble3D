#ifndef CSPHERE_H
#define CSPHERE_H

// Lib
#include "lib_math_global.h"

// Foundations
#include "CMath.h"
#include "CBox3D.h"

class CBox3D;

// Qt
#include <QVector3D>


class LIB_MATHSHARED_EXPORT CSphere
{
public:

    enum EnumBoundingMode
    {
        eInnerSphere,
        eOuterSphere
    };

	//! Constructeur
    CSphere();

	//! Constructeur
    CSphere(const QVector3D& center, real dRadius);

    //! Constructeur - Construit la sphere englobant une boite
    CSphere(const CBox3D& box, EnumBoundingMode eBoundingMode = eOuterSphere);

	//! Retourne le centre de la sphere
	QVector3D getCenter() const { return m_Center; }

	//! Retourne le rayon de la sphere
	real getRadius() const { return m_dRadius; }
	
	//! Teste l'intersection avec une sphere
	bool intersects(const CBox3D& box) const;

    //! Définit une sphére de taille nulle
    void setNull() { m_dRadius = 0.; }

    //! Retourne vrai si le rayon de la sphère est nul
    bool isNull() { return Math::isZero(m_dRadius); }

private:
	
	//! Centre
	QVector3D m_Center;

	//! Rayon
	real m_dRadius;
};

#endif // CSPHERE_H

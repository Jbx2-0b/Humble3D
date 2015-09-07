#ifndef CBOX3D_H
#define CBOX3D_H

// Lib
#include "lib_math_global.h"

// Foundations
#include "CMath.h"
#include "CRay.h"

// Qt
#include <QVector3D>
#include <QMatrix4x4>


//-----------------------------------------------------------------------------------------
// \class CBox3D
// \brief Classe permettant de definir des boites
//-----------------------------------------------------------------------------------------
class LIB_MATHSHARED_EXPORT CBox3D
{
public:

	//! Constructeur
	CBox3D()
		: m_bNeedUpdateCorners(true)
		, m_bNeedUpdateCenter(true)
	{}

	//! Constructeur
    CBox3D(const QVector3D& min, const QVector3D& max) 
		: m_vMinimumCorner(min)
		, m_vMaximumCorner(max)
		, m_bNeedUpdateCorners(true)
		, m_bNeedUpdateCenter(true)
	{}

    //! Retourne la coordonnée minimum
    QVector3D& getMinimum()					{ return m_vMinimumCorner; }

    //! Retourne la coordonnée maximum
    QVector3D& getMaximum()					{ return m_vMaximumCorner; }

    //! Retourne la coordonnée minimum
    const QVector3D& getMinimum() const		{ return m_vMinimumCorner; }

    //! Retourne la coordonnée maximum
    const QVector3D& getMaximum() const		{ return m_vMaximumCorner; }

    //! Définit la coordonnée minimum
	void setMinimum(const QVector3D& min)	{ m_vMinimumCorner = min; m_bNeedUpdateCorners = true; m_bNeedUpdateCenter = true; }

    //! Définit la coordonnée maximum
	void setMaximum(const QVector3D& max)	{ m_vMaximumCorner = max; m_bNeedUpdateCorners = true; m_bNeedUpdateCenter = true; }

	//! Définit la box à partir de deux points
	void setExtents(const QVector3D& corner1, const QVector3D& corner2);

	//! Retourne le aliste de l'ensemble des coins
	const QList<QVector3D>& getCorners() const;

    //! Retourne le centre de la boite
	QVector3D getCenter() const;

    //! Retourne la taille
    QVector3D getSize() const;

    //! Retourne la moitié de la taille
    QVector3D getHalfSize() const;

    //! Teste l'intersection avec une autre boite
	bool intersects(const CBox3D& bbox) const;

	//! Teste si un vertex est dans la boite
	bool contains(const QVector3D& vector) const;

	//! Teste si una boite est dans à l'intérieur d'une autre
	bool contains(const CBox3D& bbox) const;

	//! Realise l'union avec un point
	void unite(const QVector3D& point);

    //! Retourne l'union avec une autre boîte
	CBox3D united(const CBox3D& bbox) const;

    //! Applique la matrice de transformation à  la boite
	void transform(const QMatrix4x4& transformation);

    //! Applique la matrice de transformation à la boite
	CBox3D transformed(const QMatrix4x4& transformation) const;

    //! Ajoute un point à la boîte
	inline void merge(const QVector3D& point)
	{
		m_vMinimumCorner = Math::makeFloor(m_vMinimumCorner, point);
		m_vMaximumCorner = Math::makeCeil(m_vMaximumCorner, point);
		m_bNeedUpdateCorners = true;
		m_bNeedUpdateCenter = true;
	}

    //! Definit la boite de taille nulle
	void setNull();

	//! Retourne vrai si est nulle
	bool isNull() const;

	//! Calcule l'intersection avec un rayon
	bool intersection(const CRay& ray, real* dDistance) const;

	//! Calcule l'intersection avec un rayon
	bool intersects(const CRay &r) const;

	//! Retourne la distance minimum entre la boite et un point
	real distance(const QVector3D& v) const;

    //! Range les boîtes en fonction de leur Z Value
	static bool sortByLessZValue(const CBox3D& pBbox1, const CBox3D& pBbox2);

    //! Retourne true si les deux boîtes sont égales
	inline bool operator == (const CBox3D& bbox) const
	{
		return m_vMinimumCorner == bbox.getMinimum() && m_vMaximumCorner == bbox.getMaximum();
	}

    //! Retourne true si les deux boîtes sont égales
    inline bool operator != (const CBox3D& bbox) const
    {
        return ! operator == (bbox);
    }

	//! Affiche les propriétés de la boite de dialogue
    QString toString() const
	{
        QString result;
        result += "[Box3D ";
        result += QString("Min : %1, %2, %3, ").arg(m_vMinimumCorner.x()).arg(m_vMinimumCorner.y()).arg(m_vMinimumCorner.z());
        result += QString("Max : %1, %2, %3").arg(m_vMaximumCorner.x()).arg(m_vMaximumCorner.y()).arg(m_vMaximumCorner.z());
        result += "]";
        return result;
	}

    static const CBox3D NullBox;
    static const CBox3D NDCBox;

private:

    //! coordonnée du coin minimum
    QVector3D m_vMinimumCorner;

    //! coordonnée du coin maximum
    QVector3D m_vMaximumCorner;

	mutable QList<QVector3D> m_Corners;

	mutable QVector3D m_Center;

	mutable bool m_bNeedUpdateCorners;

	mutable bool m_bNeedUpdateCenter;

};

#endif // CBOX3D_H

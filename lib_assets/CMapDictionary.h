#ifndef CMAPDICTIONARY_H
#define CMAPDICTIONARY_H

// Lib
#include "lib_assets_global.h"

//! Foundations
#include "CMap.h"
#include "CSRTMData.h"


//! Dictionnaire de carte. 
class LIB_ASSETS_SHARED_EXPORT CMapDictionary
{

public:

	//! Constructeur par défaut
	CMapDictionary(int iCellColumnCount, int iCellRowCount, int iResolution)
		: m_iCellColumCount(iCellColumnCount)
		, m_iCellRowCount(iCellRowCount)
		, m_fXllCorner(0.)
		, m_fZllCorner(0.)
		, m_fCellSize(0.)
		, m_iMapResolution(iResolution)
		, m_fMapSize(0.)
		, m_Maps(ceil(iCellColumnCount / (real)iResolution), ceil(iCellRowCount / (real)iResolution))
	{
	}

	//! Déstructeur
	~CMapDictionary()
	{
		foreach (CMap* pMap, m_Maps.get())
		{
			delete pMap;
		}
	}

	//! Crée une carte
	CMap* createMap(const CMapKey& key) 
	{
		CMap* pMap = new CMap(key);
		m_Maps.set(key.first, key.second, pMap);

		return pMap;
	}


	//! Retourne le conteneur de cartes
	CArray2D<CMap*> maps()							{ return m_Maps; }

	//! Retourne le conteneur de cartes
	const CArray2D<CMap*>& maps() const				{ return m_Maps; }

	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! Définit la taille réelle d'une cellule en métre
    void setCellSize(real fSize)					{ m_fCellSize = fSize; }

	//! Définit la position réelle du coin sur l'axe X
    void setXCorner(real fXCorner)					{ m_fXllCorner = fXCorner; }

	//! Définit la position réelle du coin sur l'axe Z
    void setZCorner(real fZCorner)					{ m_fZllCorner = fZCorner; }

	//! Définit la taille des cartes
    void setMapSize(real fMapSize)					{ m_fMapSize = fMapSize; }

	//! Définit l'origine des positions des cartes
	void setPosition(const QVector3D& position)		{ m_vPosition = position; }

	//! Définit les altitudes
	void setHeights(const CSRTMData<int>& heights)
	{
		double dRealWidthPerMap = getRealWidthPerMap();

		m_Heights.resize(heights.width(), heights.height());

		for (int i = 0; i < heights.size(); ++i)
		{
            m_Heights.set(i, (real) heights.get(i) / dRealWidthPerMap);
		}
	}

	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne une map en fonction de sa clé
	CMap* getMap(const CMapKey& key) 				{ return getMap(key.first, key.second); }

	//! Retourne une map en fonction de sa clé
	CMap* getMap(int i, int j) 						{ return m_Maps.get(i, j); }

	//! Retourne l'altitude absolue à partir d'une position 3D
    real getAbsoluteHeight(real x, real z) const
	{
		real dAltitude = 0.;

		real dXt = (x / m_fMapSize) * (m_iMapResolution - 1);
		real dZt = (z / m_fMapSize) * (m_iMapResolution - 1);

		int iGlobalI = (int) dXt;
		int iGlobalJ = (int) dZt;

		if (iGlobalI >= 0 && iGlobalI < m_iCellColumCount && iGlobalJ >= 0 && iGlobalJ < m_iCellRowCount)
		{
			real dX = dXt - iGlobalI;	
			real dZ = dZt - iGlobalJ;
			
			real y0 = getAbsoluteHeight(iGlobalI,		iGlobalJ);
			real y1 = getAbsoluteHeight(iGlobalI + 1,	iGlobalJ);
			real y2 = getAbsoluteHeight(iGlobalI,		iGlobalJ + 1);
			real y3 = getAbsoluteHeight(iGlobalI + 1,	iGlobalJ + 1);

			if (dX > dZ)
			{
				dAltitude = y0 + dX * (y1 - y0) + dZ * (y3 - y1);
			}
			else
			{
				dAltitude = y0 + dX * (y3 - y2) + dZ * (y2 - y0);
			}
		}

		return dAltitude;
	}

	//! Retourne l'altitude absolue
    inline real getAbsoluteHeight(int iGlobalI, int iGlobalJ) const
	{
		return m_Heights.get(iGlobalI, iGlobalJ) * m_fMapSize;
	}

	//! Retourne la position 3D absolue
	inline QVector3D getAbsolutePosition(int iGlobalI, int iGlobalJ) const
	{
        real fDistanceInterPoints = m_fMapSize / (m_iMapResolution - 1);
		return QVector3D((real)iGlobalI * fDistanceInterPoints, getAbsoluteHeight(iGlobalI, iGlobalJ), (real)iGlobalJ * fDistanceInterPoints);
	}

	//! Retourne l'altitude à partir de l'origine d'une carte
    inline real getHeightInMap(const CMapKey& key, int iLocalI, int iLocalJ) const
	{
		int iGlobalI = (m_iMapResolution - 1) * key.first + iLocalI;
		int iGlobalJ = (m_iMapResolution - 1) * key.second + iLocalJ;

		if (iGlobalI > 0 && iGlobalI < m_iCellColumCount && iGlobalJ > 0 && iGlobalJ < m_iCellRowCount)
			return getAbsoluteHeight(iGlobalI, iGlobalJ);

		return 0;
	}

	//! Retourne l'altitude à partir de l'origine d'une carte
    inline real getHeightInMap(const CMapKey& key, real x, real z) const
	{
        real fGlobalX = key.first * m_fMapSize + x;
        real fGlobalZ = key.second * m_fMapSize + z;

		return getAbsoluteHeight(fGlobalX, fGlobalZ);
	}

	//! Retourne la position 3D à partir de l'origine d'une carte
	inline QVector3D getPositionInMap(const CMapKey& key, int iLocalI, int iLocalJ) const
	{
        static real fDistanceInterPoints = m_fMapSize / (m_iMapResolution - 1);
		return QVector3D((real)iLocalI * fDistanceInterPoints, getHeightInMap(key, iLocalI, iLocalJ), (real)iLocalJ * fDistanceInterPoints);
	}

	//! Retourne le nombre de map en largeur
	int getMapColumnCount() const					{ return m_Maps.width(); }

	//! Retourne le nombre de map en longueur
	int getMapRowCount() const						{ return m_Maps.height(); }

	//! Retourne le nombre de cellule en largeur
	int getCellColumnCount() const					{ return m_iCellColumCount; }

	//! Retourne le nombre de cellule en longueur
	int getCellRowCount() const						{ return m_iCellRowCount; }
	
	//! Retourne la taille réelle d'une cellule en métre
    real getCellSize() const						{ return m_fCellSize; }

	//! Définit la taille réelle d'une map en métre
    real getRealWidthPerMap() const				{ return toMeters(m_fCellSize) * (m_iMapResolution - 1); }

	//! Retourne la position réelle du coin sur l'axe X
    real getXCorner() const						{ return m_fXllCorner; }

	//! Retourne la position réelle du coin sur l'axe Z
    real getZCorner() const						{ return m_fZllCorner; }

	//! Retourne la résolution des cartes
	int getMapResolution() const					{ return m_iMapResolution; }

	//! Retourne la taille des cartes
    real getMapSize() const						{ return m_fMapSize; }

	//! Retourne l'origine des positions des cartes
	QVector3D getPosition() const					{ return m_vPosition; }

    static real toMeters(real deg)
	{
		return ((6378.137f * 1000.0f * 2.0f * Math::Pi) / 360.0f) * deg;
	}

    static real toDeg(real m)
	{
		return m / ((6378.137f * 1000.0f * 2.0f * Math::Pi) / 360.0f);
	}

private:

	int m_iCellColumCount;
	int m_iCellRowCount;
    real m_fXllCorner;
    real m_fZllCorner;
    real m_fCellSize;
	int m_iMapResolution;
    real m_fMapSize;
	QVector3D m_vPosition;

    CArray2D<real> m_Heights;

	CArray2D<CMap*> m_Maps;
};


#endif // CMAPDICTIONARY_H

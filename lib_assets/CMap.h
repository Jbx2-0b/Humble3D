#ifndef CMAP_H
#define CMAP_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CArray.h"
#include "CBox3D.h"
#include "AOctreeItem.h"

// Qt
#include <QPair>


class CMap;

typedef QPair<int, int> CMapKey;


//! Classe d�finissant une carte d�pendante d'un dictionnaire de carte
class LIB_ASSETS_SHARED_EXPORT CMap : public AOctreeItem
{
public:

	//! Constructeur par d�faut
	CMap();

	//! Constructeur
	CMap(const CMapKey& key);

    //! Destructeur
    virtual ~CMap() {}

	
	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! D�finit la bounding box
	void setBoundingBox(const CBox3D& bbox)			{ m_Bbox = bbox; }

	//! D�finit le nombre de polygones 
	void setPolygonCount(int iPolygonCount)			{ m_iPolygonCount = iPolygonCount; }
	
	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne la cl�e associ�e � la map
	CMapKey getKey() const							{ return m_Key; }
	
	//! Impl�mentation de AOctreeItem - Retourne la taille en coordonn�es moteur de la map
	virtual const CBox3D& getBoundingBox() const	{ return m_Bbox; }

	//! Impl�mentation de AOctreeItem - Retourne le nombre de polygones
	virtual int getPolygonCount() const				{ return m_iPolygonCount; }

	//-------------------------------------------------------------------------------------------------
	// 
	//-------------------------------------------------------------------------------------------------

	static const CMapKey NullKey;

protected:

	CMapKey m_Key;

	CBox3D m_Bbox;

	int m_iPolygonCount;
};


#endif // CMAP_H

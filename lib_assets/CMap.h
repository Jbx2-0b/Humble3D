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


//! Classe définissant une carte dépendante d'un dictionnaire de carte
class LIB_ASSETS_SHARED_EXPORT CMap : public AOctreeItem
{
public:

    //! Constructeur par défaut
    CMap();

    //! Constructeur
    CMap(const CMapKey& key);

    //! Destructeur
    virtual ~CMap() {}


    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit la bounding box
    void setBoundingBox(const CBox3D& bbox)			{ m_Bbox = bbox; }

    //! Définit le nombre de polygones
    void setPolygonCount(int iPolygonCount)			{ m_iPolygonCount = iPolygonCount; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne la clée associée à la map
    CMapKey getKey() const							{ return m_Key; }

    //! Implémentation de AOctreeItem - Retourne la taille en coordonnées moteur de la map
    virtual const CBox3D& getBoundingBox() const	{ return m_Bbox; }

    //! Implémentation de AOctreeItem - Retourne le nombre de polygones
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

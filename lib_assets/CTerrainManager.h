#ifndef CTERRAINMANAGER_H
#define CTERRAINMANAGER_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CSceneManager.h"
#include "CTerrain.h"
#include "CQuadtree.h"
#include "CSRTMLoader.h"

class CTerrainManager;

class CTerrainLOD
{
public:
	CTerrainLOD() {}
	CTerrainLOD(CTerrainManager* pMgr, int iLOD, float _dVisibility, int _iResolution, int iTerrainBufferSize);

	void hideAll();

    void updateTerrains(const QSet<AOctreeItem*>& proximityItems);

	CTerrain* findUnusedTerrain(const QList<CMapKey>& keys);

	void cull(CCamera* pCamera);

	CTerrainManager* m_pTerrainMgr;
	int m_iLOD;
	int m_iResolution;
	QVector<CTerrain*> m_TerrainBuffer;
	float m_dVisibility;
	QList<CMapKey> m_MapProximityKeys;
	QHash<CMapKey, CTerrain*> m_UsedTerrains;
};

//! Gestionnaire de terrains
class LIB_ASSETS_SHARED_EXPORT CTerrainManager : public CSceneManager::ICameraListener
{
	friend class CTerrainLOD;

public:

	//! Constructeur
	CTerrainManager(CSceneManager* pSceneManager, float fTerrainSize, int iTerrainResolution, int iTextureResolution, float dVisibility);

	//! Destructeur
	virtual ~CTerrainManager();
	
	//! Charge un fichier SRTM
	void loadSRTMBinaryFile(const QString& filePathName, int iQuadtreeLevel);

	//! Retourne la hauteur
	float getAbsoluteHeight(real x, real z) const		
	{
		if (m_pMapDictionary)
			return m_pMapDictionary->getAbsoluteHeight(x, z);

		return 0.;
	}

	//! Supprime l'ensemble des maps
	void clearMaps();

	//! Retourne le dictionnaire des maps
	CMapDictionary* getMapDictionary() { return m_pMapDictionary; }

protected:

	//! Implémentation de CSceneManager::ICameraListener
	virtual void onUpdateCamera(CCamera* pCamera);

private:

	CTerrain* createTerrain(int iTerrainResolution);
    void updateTerrains(const QSet<AOctreeItem*>& proximityItems, int iLOD);

	CTerrain* findUnusedTerrain(const QList<CMapKey>& keys);

	CSceneManager* m_pSceneManager;

	//! Resolution des terrains
	int m_iTerrainResolution;

	int m_iTextureResolution;

    COctree* m_pQuadtree;

	CMapDictionary* m_pMapDictionary;

	real m_fTerrainSize;

	real m_dVisibility;

	QVector<CTerrainLOD> m_TerrainLOD;

	QVector3D m_EyePosition;
};




#endif // CTERRAINMANAGER_H

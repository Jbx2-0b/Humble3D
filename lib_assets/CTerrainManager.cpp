#include "CTerrainManager.h"
#include "CFrustum.h"

// Qt
#include <QVector2D>

static const int iLODCount = 6;

//-----------------------------------------------------------------------------------------
// TERRAIN LOD
//-----------------------------------------------------------------------------------------
CTerrainLOD::CTerrainLOD(CTerrainManager* pTerrainMgr, int iLOD, float dVisibility, int iResolution, int iTerrainBufferSize)
	: m_pTerrainMgr(pTerrainMgr)
	, m_iLOD(iLOD)
	, m_iResolution(iResolution)
	, m_dVisibility(dVisibility)
{
	m_TerrainBuffer.resize(iTerrainBufferSize);

	for (int i = 0; i < m_TerrainBuffer.size(); ++i)
	{
		m_TerrainBuffer[i] = pTerrainMgr->createTerrain(m_iResolution);
	}
}

//-----------------------------------------------------------------------------------------
void CTerrainLOD::hideAll()
{
	for (int i = 0; i < m_TerrainBuffer.size(); ++i)
	{
		m_TerrainBuffer[i]->setVisible(false);
	}
}

//-----------------------------------------------------------------------------------------
void CTerrainLOD::updateTerrains(const QSet<AOctreeItem*>& proximityItems)
{
	m_MapProximityKeys.clear();

    foreach (AOctreeItem* pItem, proximityItems)
	{
		if (CMap* pMap = dynamic_cast<CMap*>(pItem))
		{
			m_MapProximityKeys << pMap->getKey();
		}
	}

	foreach (const CMapKey& key, m_MapProximityKeys)
	{
		CTerrain* pTerrain = 0;

		if (m_UsedTerrains.contains(key))
		{
			pTerrain = m_UsedTerrains[key];
		}
		else
		{
            pTerrain = findUnusedTerrain(m_MapProximityKeys);
            pTerrain->setHeights(key, m_pTerrainMgr->getMapDictionary());
			//QtConcurrent::run(pTerrain, &CTerrain::setHeights, key, m_pTerrainMgr->getMapDictionary());
            pTerrain->getNode()->resetTransformation();
            pTerrain->getNode()->translate(key.first * m_pTerrainMgr->m_fTerrainSize, 0., key.second * m_pTerrainMgr->m_fTerrainSize);

            m_UsedTerrains[key] = pTerrain;
		}
	}
}

//-----------------------------------------------------------------------------------------
CTerrain* CTerrainLOD::findUnusedTerrain(const QList<CMapKey>& keys)
{
	// Terrains pas encore utilisés
	if (m_UsedTerrains.size() < m_TerrainBuffer.size())
	{
		return m_TerrainBuffer[m_UsedTerrains.size()];
	}

	//! Terrains plus utilisés
	for (int i = 0; i < m_TerrainBuffer.size(); ++i)
	{
		CMapKey terrainKey = m_UsedTerrains.key(m_TerrainBuffer[i]);

		if (!keys.contains(terrainKey))
		{
			m_UsedTerrains.remove(terrainKey);
			return m_TerrainBuffer[i];
		}
	}

	CTerrain* pTerrain = m_pTerrainMgr->createTerrain(m_iResolution);
	m_TerrainBuffer << pTerrain;

	pLog->addMessage(eERROR, "LOD " + QString::number(m_iLOD) + " Buffer Terrain++ : " + QString::number(m_UsedTerrains.size()));
	return pTerrain;
}

//-----------------------------------------------------------------------------------------
void CTerrainLOD::cull(CCamera* pCamera)
{
	foreach (const CMapKey& key, m_MapProximityKeys)
	{
		CTerrain* pTerrain = m_UsedTerrains[key];
		pTerrain->setVisible(pCamera->isVisible(m_pTerrainMgr->getMapDictionary()->getMap(key)->getBoundingBox()));
	}
}

//-----------------------------------------------------------------------------------------
// TERRAIN MANAGER
//-----------------------------------------------------------------------------------------
CTerrainManager::CTerrainManager(CSceneManager* pSceneManager, float fTerrainSize, int iTerrainResolution, int iTextureResolution, float dVisibility)
: m_pSceneManager(pSceneManager)
, m_iTerrainResolution(iTerrainResolution + 1)
, m_iTextureResolution(iTextureResolution)
, m_pQuadtree(0)
, m_pMapDictionary(0)
, m_fTerrainSize(fTerrainSize)
, m_dVisibility(dVisibility)
{
	m_pSceneManager->registerCameraListener(this);

	m_TerrainLOD.resize(iLODCount);

	int iPrevTerrainBufferSize = 0;
	for (int i = 0; i <  iLODCount; ++i)
	{
		int iIcr = (int) pow(2., i);
		real dVisibilityLOD = i == iLODCount - 1 ? dVisibility * 2. : dVisibility / pow(2., iLODCount - i - 1);
		int iTerrainBufferSize = ((dVisibilityLOD * 2.) / m_fTerrainSize + 2) * ((dVisibilityLOD * 2.) / m_fTerrainSize + 2) - iPrevTerrainBufferSize;
		iPrevTerrainBufferSize = iTerrainBufferSize;
		m_TerrainLOD[i] = CTerrainLOD(this, i, dVisibilityLOD, iTerrainResolution / iIcr + 1, iTerrainBufferSize);
	}
}

//-----------------------------------------------------------------------------------------
CTerrainManager::~CTerrainManager()
{
	m_pSceneManager->unregisterCameraListener(this);
	clearMaps();
}

//-----------------------------------------------------------------------------------------
void CTerrainManager::loadSRTMBinaryFile(const QString& filePathName, int iQuadtreeLevel)
{
	clearMaps();

	m_pMapDictionary = CSRTMLoader::loadBinaryFile(filePathName, m_iTerrainResolution, m_fTerrainSize);

    m_pQuadtree  = new COctree(m_pMapDictionary, m_fTerrainSize, iQuadtreeLevel, iLODCount);
}

//-----------------------------------------------------------------------------------------
CTerrain* CTerrainManager::createTerrain(int iTerrainResolution)
{
	CTerrain* pTerrain = m_pSceneManager->createCustomMesh<CTerrain>("CTerrain");

	pTerrain->update(iTerrainResolution, m_iTextureResolution, QVector3D(m_fTerrainSize, m_fTerrainSize, m_fTerrainSize), false);
	pTerrain->setNotificationsEnabled(false);
	pTerrain->setVisible(false);

	CSceneNode* pTerrainNode = m_pSceneManager->getRootNode()->createChild("TerrainNode");
	pTerrainNode->addItem(pTerrain);

	return pTerrain;
}

//-----------------------------------------------------------------------------------------
void CTerrainManager::clearMaps()
{
	if (m_pQuadtree)
	{
		m_pQuadtree->clearItems();
	}

	delete m_pMapDictionary;
}


//-----------------------------------------------------------------------------------------
void CTerrainManager::onUpdateCamera(CCamera* pCamera)
{
	if (m_pQuadtree)
	{
		QVector3D eyePosition = pCamera->getEyePosition();
		if (m_EyePosition != eyePosition)
		{
			m_EyePosition = eyePosition;
		
            QSet<AOctreeItem*> proximityLODItems[iLODCount];

			CSphere3D camRange(eyePosition, m_TerrainLOD[iLODCount - 1].m_dVisibility);
			proximityLODItems[iLODCount - 1] = m_pQuadtree->getProximityItems(camRange);

			for (int i = iLODCount - 1; i > 1; --i)
			{
				CSphere3D camRangeLOD(eyePosition, m_TerrainLOD[i - 1].m_dVisibility);
                foreach (AOctreeItem* pItem, proximityLODItems[i])
				{
					pItem->setLOD(i);
					if (camRangeLOD.intersects(pItem->getBoundingBox()))
					{
						proximityLODItems[i - 1] << pItem;
						pItem->setLOD(i - 1);
						proximityLODItems[i].remove(pItem);
					}
				}
			}

			for (int i = 0; i < iLODCount; ++i)
			{
				m_TerrainLOD[i].hideAll();
				m_TerrainLOD[i].updateTerrains(proximityLODItems[i]);
			}
		}

		for (int i = 0; i < iLODCount; ++i)
		{
			m_TerrainLOD[i].cull(pCamera);
		}
	}
}


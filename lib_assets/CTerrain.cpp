#include "CTerrain.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"
#include "CShader.h"
#include "CSceneManager.h"
#include "ASceneNodeItem.h"

QMap<int, CNormalComputer> CTerrain::s_NormalComputers;
int CTerrain::s_iMaterialID = -1;

static const QString DefaultHeightMapTexture0 = "t6.png";
static const QString DefaultHeightMapTexture1 = "t1.jpg";
static const QString DefaultHeightMapTexture2 = "t4.jpg";
static const QString DefaultHeightMapTexture3 = "t4.jpg";

//-----------------------------------------------------------------------------------------
CTerrain::CTerrain()
: CMesh("Terrain")
, m_iResolution(0)
, m_iTextureResolution(0)
{
}

//-----------------------------------------------------------------------------------------
CTerrain::~CTerrain()
{
}

//-----------------------------------------------------------------------------------------
void CTerrain::update(unsigned int iResolution, unsigned int iTextureResolution, const QVector3D& size, bool bComputeNormals /*= true*/)
{
	m_iResolution = iResolution;
	m_iTextureResolution = iTextureResolution;
	m_Size = size;

	if (s_iMaterialID == -1)
	{
		m_pMaterial = CMaterialManager::getInstance()->createMaterial("Terrain");
		m_pMaterial->setAmbientColor(0.2f, 0.3f, 0.2f);
		m_pMaterial->setDiffuseColor(0.8f, 0.6f, 0.2f);
		m_pMaterial->setSpecularColor(0.1f, 0.1f, 0.1f);
		m_pMaterial->setShininessFactor(1000);

		ATexture* pTexture1 = CTextureManager::getInstance()->createTexture2D("HeightMapTex0", DefaultHeightMapTexture0);
		ATexture* pTexture2 = CTextureManager::getInstance()->createTexture2D("HeightMapTex1", DefaultHeightMapTexture1);
		ATexture* pTexture3 = CTextureManager::getInstance()->createTexture2D("HeightMapTex2", DefaultHeightMapTexture2);
		ATexture* pTexture4 = CTextureManager::getInstance()->createTexture2D("HeightMapTex3", DefaultHeightMapTexture3);


		m_pMaterial->addTexture(pTexture1, eDiffuse);
		m_pMaterial->addTexture(pTexture2, eDiffuse);
		m_pMaterial->addTexture(pTexture3, eDiffuse);
		m_pMaterial->addTexture(pTexture4, eDiffuse);

        CShader* pShader = CShaderManager::getInstance()->createShader("heightmap",
			"toon.vertex.glsl", 
			"",
			"toon.fragment.glsl");
		

		pShader->setUniformValue("fog.color", QVector4D(0.55f, 0.61f, 0.67f, 0.0f));
		pShader->setUniformValue("fog.density", 0.000003f);

		m_pMaterial->getRenderPass(0)->setShader(pShader->getName());
		m_pMaterial->getRenderPass(0)->setShader(eShadingModelUser);


		s_iMaterialID = m_pMaterial->getID();
	}
	else
	{
		m_pMaterial = CMaterialManager::getInstance()->getMaterialByID(s_iMaterialID);
	}
		
    setMaterial(m_pMaterial);

	clearMeshBuffers();

	m_pBuffer = createMeshBuffer();
	m_pBuffer->setPrimitiveType(ePrimitiveTriangles);
	m_pBuffer->setAutoComputeNormals(false);
	m_pBuffer->setUsagePattern(eDynamicDraw);

	CBuffer<QVector3D>& posBuffer = m_pBuffer->positionsBuffer();
	CBuffer<IndiceType>& idBuffer = m_pBuffer->indicesBuffer();
	CBuffer<QVector3D>& texCoordBuffer = m_pBuffer->texCoordBuffer(0);
	CBuffer<QVector3D>& nBuffer = m_pBuffer->normalsBuffer();

	posBuffer.reserve		(iResolution * iResolution);
	texCoordBuffer.reserve	(iResolution * iResolution);
	idBuffer.reserve		((iResolution - 1) * (iResolution - 1) * 6);
	nBuffer.reserve			(iResolution * iResolution);

	float fTextureStep = (float) iTextureResolution / (float) (iResolution - 1);

	float v;

	float fDistanceInterPoints = size.x() / (float) (iResolution - 1);

	for (unsigned int l = 0; l < iResolution; ++l)
	{
		float u = (float)l * fTextureStep;
		for (unsigned int w = 0; w < iResolution; ++w)
		{
			posBuffer << QVector3D((real)l * fDistanceInterPoints, 0., (real)w * fDistanceInterPoints);
		
			v = (float)w * fTextureStep;

			texCoordBuffer << QVector3D(u, v, 0.);
			
			nBuffer << QVector3D(0., 1., 0.);
		}
	}
	
	for (unsigned int l = 0; l < iResolution - 1; ++l)
	{
		for (unsigned int w = 0; w < iResolution - 1; ++w)
		{
			int iBaseIndex = l + w * m_iResolution;

			idBuffer << iBaseIndex; 
			idBuffer << iBaseIndex + iResolution + 1;
			idBuffer << iBaseIndex + iResolution;
			
			idBuffer << iBaseIndex;
			idBuffer << iBaseIndex + 1;
			idBuffer << iBaseIndex + iResolution + 1;
		}
	}

	if (bComputeNormals) computeNormals();
}

//-----------------------------------------------------------------------------------------
void CTerrain::setHeights(const CMapKey& key, CMapDictionary* pMapDictionary)
{
	int iLOD = pMapDictionary->getMap(key)->getLOD();
	int iIcr = (int) pow(2., iLOD);

	CBuffer<QVector3D>& posBuffer = m_pBuffer->positionsBuffer();

	int i = 0;
	int l, w;

	/*
	if (CMap* pMap = pMapDictionary->getMap(key.first - 1, key.second))
	{
		if (pMap->getLOD() > iLOD)
		{
			for (w = 1; w < m_iResolution - 1; ++w)
			{
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, 0,		w * iIcr));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, 0,		w * iIcr + iIcr / 2));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, iIcr,	w * iIcr));

				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, 0,		w * iIcr + iIcr / 2));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, iIcr,	(w + 1) * iIcr));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, iIcr,	w * iIcr));

				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, 0,		w * iIcr + iIcr / 2));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, 0,		(w + 1) * iIcr));
				posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, iIcr,	(w + 1) * iIcr));
			}
		}
	}*/

	/*
	if (CMap* pMap = pMapDictionary->getMap(key.first + 1, key.second))
	{
		if (pMap->getLOD() != iLOD)
		{
			bDiffLODEast = true;
		}
	}

	if (CMap* pMap = pMapDictionary->getMap(key.first, key.second - 1))
	{
		if (pMap->getLOD() != iLOD)
		{
			bDiffLODSud = true;
		}
	}

	if (CMap* pMap = pMapDictionary->getMap(key.first, key.second + 1))
	{
		if (pMap->getLOD() != iLOD)
		{
			bDiffLODNorth = true;
		}
	}*/

	for (l = 0; l < m_iResolution; ++l)
	{
		for (w = 0; w < m_iResolution; ++w)
		{
			posBuffer[i++].setY(pMapDictionary->getHeightInMap(key, l * iIcr, w * iIcr));
		}
	}
	
	if (!s_NormalComputers.contains(m_iResolution))
	{
		s_NormalComputers[m_iResolution] = CNormalComputer(m_iResolution, iLOD);
	}
	CNormalComputer& nComputer = s_NormalComputers[m_iResolution];

	CBuffer<QVector3D>& nBuffer = m_pBuffer->normalsBuffer();
	nComputer.compute(key, pMapDictionary, nBuffer);

	m_bNeedBoundingBoxUpdate = true;

	posBuffer.setDirty();
	nBuffer.setDirty();
	m_pBuffer->setDirty();
}

//-----------------------------------------------------------------------------------------
CNormalComputer::CNormalComputer()
: m_iResolution(0)
, m_iLOD(1)
{
}

//-----------------------------------------------------------------------------------------
CNormalComputer::CNormalComputer(int iResolution, int iLOD)
: m_iResolution(iResolution)
, m_iLOD(iLOD)
, normalFace1((iResolution + 1) * (iResolution + 1), QVector3D(0., 1., 0.))
, normalFace2((iResolution + 1) * (iResolution + 1), QVector3D(0., 1., 0.))
{
}

//-----------------------------------------------------------------------------------------
void CNormalComputer::compute(const CMapKey& key, CMapDictionary* pMapDictionary, CBuffer<QVector3D>& nBuffer)
{
	static QVector3D v1, v2, v3, n;
	int iIcr = (int) pow(2., m_iLOD);

	for (int l = 0; l < m_iResolution + 1; ++l)
	{
		for (int w = 0; w < m_iResolution + 1; ++w)
		{
			v1 = pMapDictionary->getPositionInMap(key, l * iIcr, w * iIcr);
			v2 = pMapDictionary->getPositionInMap(key, l * iIcr, (w + 1) * iIcr);
			v3 = pMapDictionary->getPositionInMap(key, (l + 1) * iIcr, w * iIcr);
			setNormalFace1(l, w, QVector3D::normal(v1, v2, v3));

			v1 = pMapDictionary->getPositionInMap(key, l * iIcr, (w + 1) * iIcr);
			v2 = pMapDictionary->getPositionInMap(key, (l + 1) * iIcr, (w + 1) * iIcr);
			v3 = pMapDictionary->getPositionInMap(key, (l + 1) * iIcr, w * iIcr);
			setNormalFace2(l, w, QVector3D::normal(v1, v2, v3));
		}
	}

	int i = 0;
	for (int l = 1; l < m_iResolution; ++l)
	{
		for (int w = 0; w < m_iResolution; ++w)
		{
			n = getNormalFace2(l, w) + 
				getNormalFace1(l + 1, w + 1) +
				getNormalFace1(l + 1, w) + 
				getNormalFace2(l + 1, w) +
				getNormalFace1(l, w + 1) + 
				getNormalFace2(l, w + 1);

			n.normalize();

			nBuffer[i++] = n;
		}
	}
}

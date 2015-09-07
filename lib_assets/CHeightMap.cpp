#include "CHeightMap.h"
#include "CSceneManager.h"
#include "CMath.h"
#include "CNoiseGenerator.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

#include <QImage>

//-----------------------------------------------------------------------------------------
CHeightMap::CHeightMap(real dLength,
					   real dWidth,
                       int iLengthQuadCount,
					   int iWidthQuadCount)
: CMesh("HeightMap")
, m_dLength(dLength)
, m_dWidth(dWidth)
, m_uiLineCount(iLengthQuadCount)
, m_uiColumnCount(iWidthQuadCount)
{
}

//-----------------------------------------------------------------------------------------
real CHeightMap::getAltitude(real x, real z) const
{
	real dAltitude = 0.;

    real dXt = ((m_dLength / 2. + x) / m_dLength) * (real)m_uiLineCount;
	real dZt = ((m_dWidth / 2. + z) / m_dWidth) * (real)m_uiColumnCount;

	int Xt = (int) dXt;
	int Zt = (int) dZt;

	real dX = dXt - Xt;
	real dZ = dZt - Zt;

	// On est dans le terrain ?
	if (Xt >= 0 && Xt < (int)m_uiLineCount && Zt >= 0 && Zt < (int)m_uiColumnCount)
	{	
		real y0 = getHeight(Xt, Zt);
		real y1 = getHeight(Xt + 1, Zt);
		real y2 = getHeight(Xt, Zt + 1);
		real y3 = getHeight(Xt + 1, Zt + 1);

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

//-----------------------------------------------------------------------------------------
QVector3D CHeightMap::getNormal(real x, real z) const
{
	QVector3D vNormal;

    real dXt = ((m_dLength / 2. + x) / m_dLength) * (real)m_uiLineCount;
	real dZt = ((m_dWidth / 2. + z) / m_dWidth) * (real)m_uiColumnCount;

	int Xt = (int) dXt;
	int Zt = (int) dZt;

	real dX = dXt - Xt;
	real dZ = dZt - Zt;

	// On est dans le terrain ?
	if (Xt >= 0 && Xt < (int)m_uiLineCount && Zt >= 0 && Zt < (int)m_uiColumnCount)
	{	
		QVector3D y0 = getNormal(Xt, Zt);
		QVector3D y1 = getNormal(Xt + 1, Zt);
		QVector3D y2 = getNormal(Xt, Zt + 1);
		QVector3D y3 = getNormal(Xt + 1, Zt + 1);

		if (dX > dZ)
		{
			vNormal = (y0 + dX * (y1 - y0) + dZ * (y3 - y1)).normalized();
		}
		else
		{
			vNormal = (y0 + dX * (y3 - y2) + dZ * (y2 - y0)).normalized();
		}
	}	

	return vNormal;
}

//-----------------------------------------------------------------------------------------
void CHeightMap::updateGeometry()
{
    //setSelectable(false);
	createHeightMapMeshBuffer();
	//createGrassMeshBuffer();
}

//-----------------------------------------------------------------------------------------
void CHeightMap::createHeightMapMeshBuffer()
{
    m_pHeightMapMaterial = CMaterialManager::getInstance()->createMaterial("HeightMapMaterial");
	m_pHeightMapMaterial->setAmbientColor(0.2f, 0.3f, 0.2f);
	m_pHeightMapMaterial->setDiffuseColor(0.8f, 0.6f, 0.2f);
	m_pHeightMapMaterial->setSpecularColor(0.1f, 0.1f, 0.1f);
	m_pHeightMapMaterial->setShininessFactor(1000);
	//m_pHeightMapMaterial->setTwoSided(true);

	ATexture* pTexture1 = CTextureManager::getInstance()->createTexture2D("HeightMapTex0", DefaultHeightMapTexture0);
	ATexture* pTexture2 = CTextureManager::getInstance()->createTexture2D("HeightMapTex1", DefaultHeightMapTexture1);
	ATexture* pTexture3 = CTextureManager::getInstance()->createTexture2D("HeightMapTex2", DefaultHeightMapTexture2);
	ATexture* pTexture4 = CTextureManager::getInstance()->createTexture2D("HeightMapTex3", DefaultHeightMapTexture3);
	

	m_pHeightMapMaterial->addTexture(pTexture1, eDiffuse);
	m_pHeightMapMaterial->addTexture(pTexture2, eDiffuse);
	m_pHeightMapMaterial->addTexture(pTexture3, eDiffuse);
	m_pHeightMapMaterial->addTexture(pTexture4, eDiffuse);
	
	CShader* pShader = CShaderManager::getInstance()->createShader("heightmap", 
                                               "://resources/heightmap.vertex.glsl",
											   "",
                                               "://resources/heightmap.fragment.glsl");

    pShader->setUniformValue("fog.color", QVector4D(0.75f, 0.81f, 0.87f, 0.0f));
    pShader->setUniformValue("fog.density", 0.1f);

    m_pHeightMapMaterial->getRenderPass(0)->setShaderName(pShader->getName());

    m_dCenterL = m_dLength / 2.;
	m_dCenterW = m_dWidth / 2.;

	m_Height.resize(m_uiLineCount * m_uiColumnCount);

	CNoiseGenerator2D ng(m_uiLineCount, m_uiColumnCount, iStep, iOctave);
	
	for (unsigned int l = 0; l < m_uiLineCount; ++l)
	{
		for (unsigned int w = 0 ; w < m_uiColumnCount; ++w)
		{
			setHeight(l, w, dHeightMin + ng.fCoherentNoise(l, w, dPersistance) * dHeightMax);
			//setHeight(l, w, dHeightMin + Math::randNorm() * dHeightMax);
		}
	}

	/*
	else
	{
        QImage img("heightmap.png");

		m_Height.resize(img.width() * img.height());

		for (int l = 0; l < img.width(); ++l)
		{
			for (int w = 0 ; w < img.height(); ++w)
			{
				setHeight(l, w, dHeightMin + intensity(QColor(img.pixel(l, w))) * dHeightMax);
			}
		}
	}*/

#if SMOOTHCOUNT > 0
    for (unsigned int i = 0; i < SMOOTHCOUNT; i++)
    {
        smooth();
    }
#endif

    m_pHeigthMapBuffer = createSubMesh();
	m_pHeigthMapBuffer->setPrimitiveType(ePrimitiveTriangles);
	m_pHeigthMapBuffer->setAutoComputeNormals(false);
    setMaterialName(m_pHeightMapMaterial->getName());

	CBuffer<QVector3D>& posBuffer = m_pHeigthMapBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pHeigthMapBuffer->indicesBuffer();
	CBuffer<QVector3D>& texCoordBuffer = m_pHeigthMapBuffer->texCoordBuffer(0);

	int iBaseIndex = 0;

	posBuffer.reserve((m_uiLineCount - 1) * (m_uiColumnCount - 1) * 4);
	texCoordBuffer.reserve((m_uiLineCount - 1) * (m_uiColumnCount - 1) * 4);
	idBuffer.reserve((m_uiLineCount - 1) * (m_uiColumnCount - 1) * 6);

	for (unsigned int l = 0; l < m_uiLineCount - 1; ++l)
	{
		for (unsigned int w = 0; w < m_uiColumnCount - 1; ++w)
		{
			posBuffer << getPosition(l, w);
			posBuffer << getPosition(l, w + 1);
			posBuffer << getPosition(l + 1, w);
			posBuffer << getPosition(l + 1, w + 1);

			texCoordBuffer << QVector3D(0., 0., 0.);
			texCoordBuffer << QVector3D(0., 1., 0.);
			texCoordBuffer << QVector3D(1., 0., 0.);
			texCoordBuffer << QVector3D(1., 1., 0.);

			idBuffer << iBaseIndex; 
			idBuffer << iBaseIndex + 3;
			idBuffer << iBaseIndex + 2;

			idBuffer << iBaseIndex;
			idBuffer << iBaseIndex + 1;
			idBuffer << iBaseIndex + 3;

			iBaseIndex += 4;
		}
	}

	//qDebug() << m_pHeigthMapBuffer->normalsBuffer();

	//QImage hm = ng.toImage(0.45);
	//computeNormalMap(hm);

	//ATexture* pTexture5 = CTextureManager::getInstance()->createTexture2D("HeightMapTex4", "heightmapN.png");
	//m_pHeightMapMaterial->addTexture(pTexture5, eNormals);

    computeNormals();
    computeTangents();
}

//-----------------------------------------------------------------------------------------
void CHeightMap::setHeight(int l, int w, real dValue)
{
	m_Height[l + w * m_uiLineCount] = dValue;
}

//-----------------------------------------------------------------------------------------
real CHeightMap::getHeight(int l, int w) const
{
	int iID = l + w * m_uiLineCount;

	if (iID < m_Height.size())
		return m_Height[iID];

	return 0;
}

//-----------------------------------------------------------------------------------------
QVector3D CHeightMap::getNormal(int l, int w) const
{
	int iID = l + w * m_uiLineCount;

	CBuffer<QVector3D>& nBuffer = m_pHeigthMapBuffer->normalsBuffer();

	if (iID < nBuffer.size())
		return nBuffer[iID];

	return QVector3D();
}

//-----------------------------------------------------------------------------------------
QVector3D CHeightMap::getPosition(int l, int w) const
{
    return QVector3D((real)l * m_dLength / (real)m_uiLineCount - m_dCenterL, getHeight(l, w), (real)w * m_dWidth / (real)m_uiColumnCount  - m_dCenterW);
}

//-----------------------------------------------------------------------------------------
void CHeightMap::smooth()
{
	real dBorder = 0.;

	for (unsigned int l = 1; l < m_uiLineCount - 1; ++l)
	{
		for (unsigned int w = 1; w < m_uiColumnCount - 1; ++w)
		{
			setHeight(l, w, 
				(getHeight(l - 1, w) + 
				 getHeight(l, w - 1) + 
				 getHeight(l - 1, w - 1) + 
				 getHeight(l, w) + 
				 getHeight(l + 1, w) + 
				 getHeight(l , w + 1) + 
				 getHeight(l + 1, w + 1) + 
				 getHeight(l - 1, w + 1) + 
				 getHeight(l + 1, w - 1)) 
				 / 9.);
		}
	}

	for (unsigned int l = 0; l < m_uiLineCount; ++l)
	{
		setHeight(l, 0, dBorder);
		setHeight(l, m_uiColumnCount - 1, dBorder);
	}

	for (unsigned int l = 0; l < m_uiColumnCount; ++l)
	{
		setHeight(0, l, dBorder);
		setHeight(m_uiLineCount - 1, l, dBorder);
	}
}

//-----------------------------------------------------------------------------------------
void CHeightMap::computeNormals()
{
	// Structure locale pour calculer les normales
	struct SNormalComputer
	{
        SNormalComputer(int iLengthQuadCount, int iWidthQuadCount, CHeightMap* pHeightMap)
            : m_iLength(iLengthQuadCount)
			, m_iWidth(iWidthQuadCount)
            , normalFace1(iLengthQuadCount * iWidthQuadCount, QVector3D(0., 1., 0.))
            , normalFace2(iLengthQuadCount * iWidthQuadCount, QVector3D(0., 1., 0.))
		{
			QVector3D v1, v2, v3;

            for (int l = 0; l < m_iLength - 1; ++l)
			{
				for (int w = 0; w < m_iWidth - 1; ++w)
				{
					v1 = pHeightMap->getPosition(l, w);
					v2 = pHeightMap->getPosition(l, w + 1);
					v3 = pHeightMap->getPosition(l + 1, w);
					setNormalFace1(l, w, QVector3D::normal(v1, v2, v3));

					v1 = pHeightMap->getPosition(l, w + 1);
					v2 = pHeightMap->getPosition(l + 1, w + 1);
					v3 = pHeightMap->getPosition(l + 1, w);
					setNormalFace2(l, w, QVector3D::normal(v1, v2, v3));
				}
			}

            m_Normals.resize(m_iLength * m_iWidth);
			m_Normals.fill(QVector3D(0., 1., 0.));

			QVector3D n;

            for (int l = 1; l < m_iLength - 1; ++l)
			{
				for (int w = 1; w < m_iWidth - 1; ++w)
				{
					n = getNormalFace2(l, w) + 
						getNormalFace1(l + 1, w + 1) +
						getNormalFace1(l + 1, w) + 
						getNormalFace2(l + 1, w) +
						getNormalFace1(l, w + 1) + 
						getNormalFace2(l, w + 1);

					n.normalize();

					setNormal(l, w, n);
				}
			}
		}

        void setNormalFace1(int l, int w, const QVector3D& v)	{ normalFace1[l + w * m_iLength] = v; }
        void setNormalFace2(int l, int w, const QVector3D& v)	{ normalFace2[l + w * m_iLength] = v; }

        QVector3D getNormalFace1(int l, int w) const			{ return normalFace1[l + w * m_iLength]; }
        QVector3D getNormalFace2(int l, int w) const			{ return normalFace2[l + w * m_iLength]; }

        void setNormal(int l, int w, const QVector3D& v)		{ m_Normals[l + w * m_iLength] = v; }

        QVector3D getNormal(int l, int w) const					{ return m_Normals[l + w * m_iLength]; }

        int m_iLength;
		int m_iWidth;

		QVector<QVector3D> normalFace1;
		QVector<QVector3D> normalFace2;
		CBuffer<QVector3D> m_Normals;
	}
	normalComputer(m_uiLineCount, m_uiColumnCount, this);

	CBuffer<QVector3D>& nBuffer = m_pHeigthMapBuffer->normalsBuffer();

	for (unsigned int l = 0; l < m_uiLineCount - 1; ++l)
	{
		for (unsigned int w = 0; w < m_uiColumnCount - 1; ++w)
		{
			nBuffer << normalComputer.getNormal(l, w);
			nBuffer << normalComputer.getNormal(l, w + 1);
			nBuffer << normalComputer.getNormal(l + 1, w);
			nBuffer << normalComputer.getNormal(l + 1, w + 1);
		}
	}
}

//-----------------------------------------------------------------------------------------
CHeightMap::~CHeightMap()
{
}

//-----------------------------------------------------------------------------------------
QImage CHeightMap::computeNormalMap(const QImage& heightmap, double pStrength)
{
	// assume square texture, not necessarily true in real code
	QImage result(heightmap.width(), heightmap.height(), QImage::Format_RGB32);

	int textureSize = heightmap.width();

	CBuffer<QVector3D>& nBuffer = m_pHeigthMapBuffer->normalsBuffer();
	nBuffer.resize(textureSize * textureSize);

	for (int row = 0; row < textureSize; ++row)
	{
		for (int column = 0; column < textureSize; ++column)
		{
			// surrounding pixels
			const QRgb topLeft =		heightmap.pixel(Math::clamp(row - 1, textureSize - 1),	Math::clamp(column - 1, textureSize - 1));
			const QRgb top =			heightmap.pixel(Math::clamp(row - 1, textureSize - 1),	Math::clamp(column, textureSize - 1));
			const QRgb topRight =		heightmap.pixel(Math::clamp(row - 1, textureSize - 1),	Math::clamp(column + 1, textureSize - 1));
			const QRgb right =			heightmap.pixel(Math::clamp(row, textureSize - 1),		Math::clamp(column + 1, textureSize - 1));
			const QRgb bottomRight =	heightmap.pixel(Math::clamp(row + 1, textureSize - 1),	Math::clamp(column + 1, textureSize - 1));
			const QRgb bottom =			heightmap.pixel(Math::clamp(row + 1, textureSize - 1),	Math::clamp(column, textureSize - 1));
			const QRgb bottomLeft =		heightmap.pixel(Math::clamp(row + 1, textureSize - 1),	Math::clamp(column - 1, textureSize - 1));
			const QRgb left =			heightmap.pixel(Math::clamp(row, textureSize - 1),		Math::clamp(column - 1, textureSize - 1));

			// their intensities
			const double tl = intensity(topLeft);
			const double t = intensity(top);
			const double tr = intensity(topRight);
			const double r = intensity(right);
			const double br = intensity(bottomRight);
			const double b = intensity(bottom);
			const double bl = intensity(bottomLeft);
			const double l = intensity(left);

			// sobel filter
			const double dX = (tr + 2.0 * r + br) - (tl + 2.0 * l + bl);
			const double dY = (bl + 2.0 * b + br) - (tl + 2.0 * t + tr);
			const double dZ = 1.0 / pStrength;

			QVector3D v(dX, dY, dZ);
			v.normalize();

			nBuffer[row + column * textureSize] = v;

			// convert to rgb
			//result.setPixel(row, column, color.rgb());
		}
	}

	return result;
}

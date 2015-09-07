#ifndef CNOISEHEIGHTMAP_H
#define CNOISEHEIGHTMAP_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"
#include "CMaterial.h"

// Textures
static const QString DefaultHeightMapTexture0 = "://resources/t6.jpg";
static const QString DefaultHeightMapTexture1 = "://resources/t1.jpg";
static const QString DefaultHeightMapTexture2 = "://resources/t4.jpg";
static const QString DefaultHeightMapTexture3 = "://resources/Detail2.jpg";

// Heightmap
static const real DefaultHeightMapSize = 200;
static const unsigned int DefaultLineCount = 150;
static const real dHeightMin = -10.;
static const real dHeightMax = 30.;
static const unsigned int iOctave = 10;
static const unsigned int iStep = 80;
static const double dPersistance = 0.5;
static const unsigned int SMOOTHCOUNT = 0;


class LIB_ASSETS_SHARED_EXPORT CNoiseHeightMap : public CMesh
{

public:

	//! Constructeur
    CNoiseHeightMap(real dLength = DefaultHeightMapSize,
			   real dWidth =  DefaultHeightMapSize,
			   int iLineCount = DefaultLineCount,
			   int iColumnCount = DefaultLineCount);

	//! Destructeur
    virtual ~CNoiseHeightMap();

	//! Retourne l'altitude de l'heightmap pour un point (x, z)
	real getAltitude(real x, real z) const;

	//! Retourne la normale de l'heightmap pour un point (x, z)
	QVector3D getNormal(real x, real z) const;

	//! Calcule les normales
	virtual void computeNormals();

	real getLength() const { return m_dLength; }

	real getWidth() const { return m_dWidth; }

protected:

	virtual void updateGeometry();

	void createHeightMapMeshBuffer();

	inline void addBlade(real dL, real dW, real dRotX, real dRotY, real dRotZ, real dScaleY);
	
	void setHeight(int l, int w, real dValue);

	real getHeight(int l, int w) const;

	QVector3D getNormal(int l, int w) const;

	QVector3D getPosition(int l, int w) const;

	void smooth();

	QImage computeNormalMap(const QImage& heightmap, double pStrength = 4.0);

private:
	
    real intensity(const QColor& color)
    {
        return ((color.red() + color.green() + color.blue()) / 3.) / 255.;
    }

    QVector<real> m_Height;
	
	CMaterial* m_pHeightMapMaterial;

    CMeshBuffer* m_pHeigthMapBuffer;

	real m_dLength;
	real m_dWidth;
	real m_dCenterL;
	real m_dCenterW;
	unsigned int m_uiLineCount;
	unsigned int m_uiColumnCount;
};

#endif // CNOISEHEIGHTMAP_H

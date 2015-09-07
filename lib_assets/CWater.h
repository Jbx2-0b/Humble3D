#ifndef CWATER_H
#define CWATER_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"

class CMaterial;

static const real DefaultWaterSize = 1000000;
static const unsigned int DefaultWaterQuadCount = 1000;
static const real DefaultWaterHeight = -50.f;
static const int DefaultNumWaves = 8;

class LIB_ASSETS_SHARED_EXPORT CWater : public CMesh
{

public:

	//! Constructeur
    CWater(real dLength = DefaultWaterSize,
           real dWidth =  DefaultWaterSize,
           int iLengthQuadCount = DefaultWaterQuadCount,
           int iWidthSquareCount = DefaultWaterQuadCount);

	//! Destructeur
	virtual ~CWater();

protected:

	virtual void updateGeometry();

	QVector3D getPosition(int l, int w) const;

private:

	CMaterial* m_pMaterial;
    CSubMesh* m_pBuffer;

    real m_dLength;
	real m_dWidth;
	real m_dCenterL;
	real m_dCenterW;
    unsigned int m_iLengthQuadCount;
	unsigned int m_iWidthQuadCount;
};

#endif // CWATER_H

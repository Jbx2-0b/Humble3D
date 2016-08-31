#ifndef CGRASS_H
#define CGRASS_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMeshInstance.h"


// Texture
static const QString DefaultDiffuseGrassTexture = "grass_diffuse.png";
static const QString DefaultNormalsTexture = "grass_norm.png";

// Blade of grass
static const real dBladeHeight = 0.15f;
static const real dBladeWidth = 0.008f;
static const real dF1stFloorW = 4.5f/5.f;
static const real dF2ndFloorW = 3.f/4.f;
static const real dF1stFloorH = 1.f/2.f;
static const real dF2ndFloorH = 4.f/5.f;
static const real dZShift = 0.02f;

// Grass
static const real dBladeStep = 0.08f;

class CHeightMap;


class LIB_ASSETS_SHARED_EXPORT CGrass : public CMesh
{
public:
    CGrass();

    virtual ~CGrass();

    void setHeightMap(CHeightMap* pHeightMap);

protected:

    virtual void updateGeometry();

private:

    CHeightMap* m_pHeightMap;
    CSubMesh* m_pGrassBuffer;
    CMaterial* m_pGrassMaterial;
};

#endif // CGRASS_H

#ifndef CSKYBOX_H
#define CSKYBOX_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"

class CMaterial;

static const real dDefaultSkyBoxSize = 1.;

class LIB_ASSETS_SHARED_EXPORT CSkyBox : public CMesh
{

public:

    //! Constructeur
    CSkyBox();

    //! Destructeur
    virtual ~CSkyBox();

protected:

    virtual void updateGeometry();
};

#endif // CSKYBOX_H

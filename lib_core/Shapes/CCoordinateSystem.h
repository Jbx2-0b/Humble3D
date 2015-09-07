#ifndef CCOORDINATESYSTEM_H
#define CCOORDINATESYSTEM_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"
#include "CGeometryGlobal.h"

class LIB_CORE_SHARED_EXPORT CCoordinateSystem : public CMesh
{
public:

    enum EnumArrowAxis
    {
        eArrowAxisX,
        eArrowAxisY,
        eArrowAxisZ,
    };

    //! Constructeur
    CCoordinateSystem();

    //! Destructeur
    virtual ~CCoordinateSystem();

protected:

    virtual void updateGeometry();

private:

    void initArrow(CSubMesh* pArrow, const Color& color, EnumArrowAxis eArrowAxis);
};

#endif // CCOORDINATESYSTEM_H

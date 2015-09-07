#ifndef CCYLINDERMESH_H
#define CCYLINDERMESH_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"
#include "CMesh.h"


static const int DefaultCylinderSegmentCount = 64;

class LIB_CORE_SHARED_EXPORT CCylinderMesh : public CMesh
{
public:

	//! Constructeur
    CCylinderMesh(real dRadius = 0.5, real dHeight = 1., int iSegmentCount = DefaultCylinderSegmentCount);

	//! Destructeur
    virtual ~CCylinderMesh();

protected:

	virtual void updateGeometry();

private:
	
    CSubMesh* m_pCylinderBuffer;
	real m_dRadius;
	real m_dHeight; 
	int m_iSegmentCount;
};

#endif // CCYLINDERMESH_H

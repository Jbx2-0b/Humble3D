#ifndef CLINEBOX_H
#define CLINEBOX_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"
#include "CBox3D.h"


class LIB_CORE_SHARED_EXPORT CLineBox : public CMesh
{
public:

    CLineBox(const QVector3D& size = QVector3D(1.0, 1.0, 1.0));

	void setSize(const QVector3D& size);

    void setBoundingBox(const CBox3D& bbox);

    virtual ~CLineBox();

protected:

    virtual void updateGeometry();

    CSubMesh* m_pCubeBuffer;
    QVector3D m_vMin, m_vMax;
};


#endif // CLINEBOX_H

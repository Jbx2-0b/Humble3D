#ifndef CPYRAMID_H
#define CPYRAMID_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"


class LIB_CORE_SHARED_EXPORT CPyramid : public CMesh
{
public:

    CPyramid(const QVector3D& size = QVector3D(1.0, 1.0 / Math::Sqrt2, 1.0));

    void setSize(const QVector3D& size);

    QVector3D getSize() const { return m_Size; }

    virtual ~CPyramid();

protected:

    virtual void updateGeometry();

    CSubMesh* m_pBuffer;

    QVector3D m_Size;
};


#endif // CPYRAMID_H

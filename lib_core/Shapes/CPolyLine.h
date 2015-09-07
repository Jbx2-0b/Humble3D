#ifndef CPOLYLINE_H
#define CPOLYLINE_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"


class LIB_CORE_SHARED_EXPORT CPolyLine : public CMesh
{
public:

    CPolyLine();

    void addPoints(const QList<QVector3D>& pts) { m_Points << pts; updateGeometry(); }

    QList<QVector3D>& points() { return m_Points; }

    const QList<QVector3D>& points() const { return m_Points; }

    void setClosed(bool bClosed) { m_bClosed = bClosed; updateGeometry(); }

    CPolyLine& operator << (const QList<QVector3D>& pts)
    {
        m_Points << pts;
        update();
        return *this;
    }

    CPolyLine& operator << (const CPolyLine& polyLine)
    {
        m_Points << polyLine.points();
        update();
        return *this;
    }

protected:

    virtual void updateGeometry();

    QList<QVector3D> m_Points;

    CSubMesh* m_pBuffer;

    bool m_bClosed;

};

#endif // CPOLYLINE_H

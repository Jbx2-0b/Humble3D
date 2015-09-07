#include "CFrustum.h"


//-----------------------------------------------------------------------------------------
CFrustum::CFrustum(const QMatrix4x4& modelViewProjectionMatrix)
{
    update(modelViewProjectionMatrix);
}

//-----------------------------------------------------------------------------------------
void CFrustum::update(const QMatrix4x4& mvp)
{
    m_Planes[eLeft] = CPlane(
            mvp(3, 0) + mvp(0, 0),
            mvp(3, 1) + mvp(0, 1),
            mvp(3, 2) + mvp(0, 2),
            mvp(3, 3) + mvp(0, 3));

    m_Planes[eRight] = CPlane(
            mvp(3, 0) - mvp(0, 0),
            mvp(3, 1) - mvp(0, 1),
            mvp(3, 2) - mvp(0, 2),
            mvp(3, 3) - mvp(0, 3));

    m_Planes[eTop] = CPlane(
            mvp(3, 0) - mvp(1, 0),
            mvp(3, 1) - mvp(1, 1),
            mvp(3, 2) - mvp(1, 2),
            mvp(3, 3) - mvp(1, 3));

    m_Planes[eBottom] = CPlane(
            mvp(3, 0) + mvp(1, 0),
            mvp(3, 1) + mvp(1, 1),
            mvp(3, 2) + mvp(1, 2),
            mvp(3, 3) + mvp(1, 3));

    m_Planes[eNear] = CPlane(
            mvp(3, 0) + mvp(2, 0),
            mvp(3, 1) + mvp(2, 1),
            mvp(3, 2) + mvp(2, 2),
            mvp(3, 3) + mvp(2, 3));

    m_Planes[eFar] = CPlane(
            mvp(3, 0) - mvp(2, 0),
            mvp(3, 1) - mvp(2, 1),
            mvp(3, 2) - mvp(2, 2),
            mvp(3, 3) - mvp(2, 3));

    for(int i = 0; i < 6; i++)
    {
        m_Planes[i].normalize();
//        real length = m_Planes[i].getNormal().length();
//        m_Planes[i].getNormal().normalize();
//        m_Planes[i].setConstant(m_Planes[i].getConstant() / length);
    }
}

//-----------------------------------------------------------------------------------------
EnumIntersectionType CFrustum::isPointInFrustum(const QVector3D& point) const
{
	EnumIntersectionType eResult = eInside;

	for (int i = 0; i < 6; i++)
	{
        if (m_Planes[i].getDistance(point) < 0)
			return eOutside;
	}

	return eResult;
}

//-----------------------------------------------------------------------------------------
EnumIntersectionType CFrustum::isSphereInFrustum(const CSphere& sphere) const
{
	real dDistance;
	EnumIntersectionType eResult = eInside;

	for(int i=0; i < 6; i++)
	{
        dDistance = m_Planes[i].getDistance(sphere.getCenter());
        if (dDistance < -sphere.getRadius())
			return eOutside;
        else if (dDistance < sphere.getRadius())
			eResult =  eIntersect;
	}

	return eResult;
}

//-----------------------------------------------------------------------------------------
EnumIntersectionType CFrustum::isBoxInFrustum(const CBox3D& box) const
{
    EnumIntersectionType eResult = eInside;
    int out, in;

    QList<QVector3D> corners = box.getCorners();

    // for each plane do ...
    for (int i = 0; i < 6; i++)
    {
        // reset counters for corners in and out
        out = 0; in = 0;
        // for each corner of the box do ...
        // get out of the cycle as soon as a box as corners
        // both inside and out of the frustum
        for (int k = 0; k < 8 && (in == 0 || out == 0); k++)
        {
            // is the corner outside or inside
            if (m_Planes[i].getDistance(corners[k]) < 0)
                out++;
            else
                in++;
        }
        //if all corners are out
        if (!in)
        {
            return eOutside;
        }
        else if (out)
        {
            eResult = eIntersect;
        }
    }
    return eResult;
}

//-----------------------------------------------------------------------------------------
bool CFrustum::isVisible(const CBox3D& box) const
{
    // Get centre of the box
    QVector3D centre = box.getCenter();
    // Get the half-size of the box
    QVector3D halfSize = box.getHalfSize();

    // For each plane, see if all points are on the negative side
    // If so, object is not visible
    for (int plane = 0; plane < 6; ++plane)
    {
        CPlane::EnumSide eSide = m_Planes[plane].getSide(centre, halfSize);
        if (eSide == CPlane::eNegativeSide)
        {
            return false;
        }
    }
    return true;
}

#ifndef CGEOMETRYTOOL_H
#define CGEOMETRYTOOL_H

#include "CMath.h"
#include "CMeshBuffer.h"

class CGeometryTool
{
public:
    static void quad(CMeshBuffer& meshBuffer, real x1, real y1, real x2, real y2, real x3, real y3, real x4, real y4);
    static void extrude(CMeshBuffer& meshBuffer, real x1, real y1, real x2, real y2);
};

#endif // CGEOMETRYTOOL_H

#ifndef CGLTEXTURETARGET_H
#define CGLTEXTURETARGET_H

#include "AGLTexture.h"
#include "CGeometryGlobal.h"

class CGLTextureTarget : public AGLTexture
{
public:

    CGLTextureTarget(int iWidth, int iHeight, EnumInternalFormat eFormat);
};

#endif // CGLTEXTURETARGET_H

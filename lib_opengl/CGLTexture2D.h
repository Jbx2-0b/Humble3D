#ifndef CGLTEXTURE2D_H
#define CGLTEXTURE2D_H

#include "AGLTexture.h"

class CGLTexture2D : public AGLTexture
{
public:

	CGLTexture2D(int iWidth, int iHeight);
	CGLTexture2D(const QString& fileName, int iWidth = 0, int iHeight = 0);

	void loadSub(const QString& fileName, const QRect& rect);
};

#endif // CGLTEXTURE2D_H

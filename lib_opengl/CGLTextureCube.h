#ifndef CGLTEXTURECUBE_H
#define CGLTEXTURECUBE_H

#include "AGLTexture.h"

class CGLTextureCube : public AGLTexture
{

public:

	CGLTextureCube(const QStringList& fileNames);
};

#endif // CGLTEXTURECUBE_H

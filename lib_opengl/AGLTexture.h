#ifndef AGLTEXTURE_H
#define AGLTEXTURE_H

#include "CGLGlobal.h"

class AGLTexture : protected CGLFunctions
{
public:

    AGLTexture();
    virtual ~AGLTexture() = 0;

    virtual bool isValid()					{ return m_bValid; }
    virtual unsigned int getID() const		{ return m_iTextureID; }
    virtual unsigned int getWidth() const	{ return m_iWidth; }
    virtual unsigned int getHeight() const	{ return m_iHeight; }

    virtual void bind();
    virtual void release();

    void generateMipMaps();

    static QImage convertToGLFormat(const QImage& img);

protected:

    // Disable copy constructor
    AGLTexture(const AGLTexture&);

    // Disable affectation operator
    AGLTexture& operator=(const AGLTexture& buffer);

    GLuint m_iTextureID;
    bool m_bValid;
    int m_iType;
    unsigned int m_iWidth;
    unsigned int m_iHeight;
};

#endif // AGLTEXTURE_H

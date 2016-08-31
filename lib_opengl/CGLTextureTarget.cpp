#include "CGLTextureTarget.h"
#include "CLogManager.h"
#include "CGLHelper.h"

//-----------------------------------------------------------------------------------------
CGLTextureTarget::CGLTextureTarget(int iWidth, int iHeight, EnumInternalFormat eFormat)
{
    m_iType = GL_TEXTURE_2D;
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    glBindTexture(GL_TEXTURE_2D, m_iTextureID);

    // TODO : set format & attachment
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // On alloue un espace pour les futures donnees
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, CGLHelper::toGLType(eFormat), GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_bValid = true;
}

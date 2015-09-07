#include "CGLTexture2D.h"
#include "CLogManager.h"

// Qt
#include <QImage>

//-----------------------------------------------------------------------------------------
CGLTexture2D::CGLTexture2D(int iWidth, int iHeight)
{
    m_iType = GL_TEXTURE_2D;
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    glBindTexture(GL_TEXTURE_2D, m_iTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_bValid = true;
}

//-----------------------------------------------------------------------------------------
CGLTexture2D::CGLTexture2D(const QString& fileName, int iWidth, int iHeight)
{
    m_iType = GL_TEXTURE_2D;
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    if (!QFile::exists(fileName))
    {
        m_bValid = false;
        pLog->addMessage(eWARN, "CGLTexture2D::CGLTexture2D() : Texture introuvable: " + fileName);
        return;
    }

    QImage image(fileName);

    if (image.isNull())
    {
        m_bValid = false;
        pLog->addMessage(eWARN, "CGLTexture2D::CGLTexture2D() : Erreur chargement de la texture: " + fileName);
        return;
    }

    image = convertToGLFormat(image);

    iWidth = image.width();

    if (iHeight <= 0)
    {
        iHeight = image.height();
    }

    if (iWidth != image.width() || iHeight != image.height())
    {
        image = image.scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    m_iWidth = image.width();
    m_iHeight = image.height();

    glBindTexture(GL_TEXTURE_2D, m_iTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    generateMipMaps();
    glBindTexture(GL_TEXTURE_2D, 0);

    m_bValid = true;
}

//-----------------------------------------------------------------------------------------
void CGLTexture2D::loadSub(const QString& fileName, const QRect& rect)
{
    if (!QFile::exists(fileName))
    {
        pLog->addMessage(eWARN, "CGLTexture2D::loadSub() : Texture introuvable: " + fileName);
        m_bValid = false;
        return;
    }

    QImage image(fileName);

    if (image.isNull())
    {
        pLog->addMessage(eWARN, "CGLTexture2D::loadSub() : Erreur chargement de la texture: " + fileName);
        m_bValid = false;
        return;
    }

    image = convertToGLFormat(image);

    glBindTexture(GL_TEXTURE_2D, m_iTextureID);

    glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x(), rect.y(), rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    generateMipMaps();
    glBindTexture(GL_TEXTURE_2D, 0);

    m_bValid = true;
}

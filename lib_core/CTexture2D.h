#ifndef CTEXTURE2D_H
#define CTEXTURE2D_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CGeometryGlobal.h"
#include "AEntity.h"
#include "ATexture.h"

// Qt
#include <QString>
#include <QStringList>
#include <QImage>


typedef QPair<QString, QRect> SImageRect;


class LIB_CORE_SHARED_EXPORT CTexture2D : public ATexture
{
public:

    //! Constructeur
    CTexture2D(const QString& name, int iTextureUnit)
        : ATexture(name, eTexture2D, iTextureUnit)
    {
    }

    //! Constructeur
    CTexture2D(const QString& name, int iTextureUnit, const QString& fileName)
        : ATexture(name, eTexture2D, iTextureUnit)
    {
        m_Size = QImage(fileName).size();
        m_ImageRects << SImageRect(fileName, QRect(0, 0, m_Size.width(), m_Size.height()));

        m_bIsValid = true;
    }

    //! Constructeur
    CTexture2D(const QString& name, int iTextureUnit, const QSize& size)
        : ATexture(name, eTexture2D, iTextureUnit)
        , m_Size(size)
    {
        m_bIsValid = true;
    }

    void setFileName(const QString& fileName)
    {
        m_ImageRects.clear();
        m_Size = QImage(fileName).size();
        m_ImageRects << SImageRect(fileName, QRect(0, 0, m_Size.width(), m_Size.height()));
        m_bIsValid = true;
        notifyUpdate();
    }

    void addImageRect(const SImageRect& imgRect)		{ m_ImageRects << imgRect; notifyUpdate(); }

    void clearImageRects()								{ m_ImageRects.clear(); notifyUpdate(); }

    const QList<SImageRect>& imageRects() const			{ return m_ImageRects; }

    //! Retourne le nom du type
    virtual QString getTypeName() const					{ return "Texture2D"; }

    //! Retourne la taille de la texture
    QSize getSize() const								{ return m_Size; }

    //! Retourne le nom de fichier image
    QString getFileName() const							{ return m_ImageRects.size() ? m_ImageRects[0].first : ""; }

        //! Affiche les propriétés sur la console
        virtual QString toString() const
        {
        QString result;
                                                          result += "[Texture 2D\n";
                                                                                                            ATexture::toString();
                                                                                                                                                              foreach (const SImageRect& imageRect, m_ImageRects)
                                                                                                                                                              {
                                                                                                                                                                  result += QString("Image Rect: %1 (%2, %3, %4x%5)\n")
                                                                                                                                                                          .arg(imageRect.first)
                                                                                                                                                                          .arg(imageRect.second.x())
                                                                                                                                                                          .arg(imageRect.second.y())
                                                                                                                                                                          .arg(imageRect.second.width())
                                                                                                                                                                          .arg(imageRect.second.height());
                                                                                                                                                              }

                                                                                                                                                                                                                result += QString("Size: %1x%2\n").arg(m_Size.width()).arg(m_Size.height());

                                                                                                                                                                                                                                                                  return result;
                                                        }

protected:

    QList<SImageRect> m_ImageRects;

    //! Taille de la texture
    QSize m_Size;
};


#endif // CTEXTURE2D_H

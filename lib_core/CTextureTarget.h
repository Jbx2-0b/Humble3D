#ifndef CTEXTURETARGET_H
#define CTEXTURETARGET_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CGeometryGlobal.h"
#include "AEntity.h"
#include "ATexture.h"

// Qt
#include <QString>


class LIB_CORE_SHARED_EXPORT CTextureTarget : public ATexture
{
public:

	//! Constructeur
	CTextureTarget(const QString& name, int iTextureUnit, EnumInternalFormat eFormat)
        : ATexture(name, eTextureTarget, iTextureUnit)
        , m_eFormat(eFormat)
    {
	}

	//! Constructeur
	CTextureTarget(const QString& name, int iTextureUnit, const QSize& size, EnumInternalFormat eFormat)
        : ATexture(name, eTextureTarget, iTextureUnit)
		, m_Size(size)
        , m_eFormat(eFormat)
    {
		m_bIsValid = true;
	}

	//! Retourne le nom du type
	virtual QString getTypeName() const					{ return "TextureTarget"; }

	//! Retourne la taille de la texture
	QSize getSize() const								{ return m_Size; }
	
    //! Retourne le format de la texture
    EnumInternalFormat getFormat() const				{ return m_eFormat; }

	//! Affiche les propriétés sur la console
    virtual QString toString() const
	{
		QString result;
		result += "[Texture Target\n";
		ATexture::toString();
		result += QString("Size: %1x%2\n").arg(m_Size.width()).arg(m_Size.height());

		return result;
	}

protected:
	//! Taille de la texture
	QSize m_Size;

    //! Format de la texture
    EnumInternalFormat m_eFormat;
};


#endif // CTEXTURETARGET_H

#ifndef CTEXTURE3D_H
#define CTEXTURE3D_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "ATexture.h"

// Qt
#include <QString>

class LIB_CORE_SHARED_EXPORT CTexture3D : public ATexture
{
public:
	

	//! Retourne le nom du type
	virtual QString getTypeName() const					{ return "Texture3D"; }

	//! Affiche les propriétés sur la console
    virtual QString toString() const
	{
		QString result;
		result += "[Texture 3D\n";
		ATexture::toString();
		return result;
	}
};


#endif // CTEXTURE3D_H

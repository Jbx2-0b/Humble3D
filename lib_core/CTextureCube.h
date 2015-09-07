#ifndef CTEXTURECUBE_H
#define CTEXTURECUBE_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "ATexture.h"
#include "CGeometryGlobal.h"


class LIB_CORE_SHARED_EXPORT CTextureCube : public ATexture
{
public:

    //! Constructeur [FileNames order : posX, negX, posY, negY, posZ, negZ]
    CTextureCube(const QString& name, int iTextureUnit, const QStringList& fileNames)
        : ATexture(name, eTextureCube, iTextureUnit)
        , m_FileNames(fileNames)
    {
        m_bIsValid = true;
    }

    //! Constructeur
    CTextureCube(const QString& name,
                 int iTextureUnit,
                 const QString& fileNamePosX,
                 const QString& fileNameNegX,
                 const QString& fileNamePosY,
                 const QString& fileNameNegY,
                 const QString& fileNamePosZ,
                 const QString& fileNameNegZ)
        : ATexture(name, eTextureCube, iTextureUnit)
    {
        m_FileNames << fileNamePosX << fileNameNegX << fileNamePosY << fileNameNegY << fileNamePosZ << fileNameNegZ;
        m_bIsValid = true;
    }

    //! Retourne le nom du type
    virtual QString getTypeName() const					{ return "TextureCube"; }

    //! Retourne les noms de fichier image
    const QStringList& getFileNames() const				{ return m_FileNames; }

    //! Définit les noms de fichier image
    void setFileNames(const QStringList& fileNames)		{ m_FileNames = fileNames; }

    //! Affiche les propriétés sur la console
    virtual QString toString() const
    {
        QString result;
        result += "[Texture Cube\n";
        ATexture::toString();
        return result;
    }

protected:
    //! Noms des fichiers images
    QStringList m_FileNames;
};

#endif // CTEXTURECUBE_H

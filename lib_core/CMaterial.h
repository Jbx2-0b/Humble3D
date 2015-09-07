#ifndef CMATERIAL_H
#define CMATERIAL_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"
#include "CGeometryGlobal.h"
#include "AEntity.h"
#include "ATexture.h"
#include "CTextureManager.h"
#include "CStringHelper.h"
#include "CRenderPass.h"

// Qt
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector4D>


class CTextureParam
{
public:

    CTextureParam() : m_eMaterialParameter(eUndefined) {}

    CTextureParam(const QString& textureName, EnumMaterialParameter eMaterialParameter)
        : m_TextureName(textureName), m_eMaterialParameter(eMaterialParameter) {}

    QString getTextureName() const { return m_TextureName; }

    EnumMaterialParameter getMaterialParameter() const { return m_eMaterialParameter; }

    // Comparison operators
    bool operator == (const CTextureParam& o) const
    {
        return o.m_TextureName == m_TextureName;
    }

private:
    QString m_TextureName;
    EnumMaterialParameter m_eMaterialParameter;
};


//-----------------------------------------------------------------------------------------
// \class CMaterial
// \brief Classe permettant de definir un materiau
//-----------------------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT CMaterial : public AEntity
{
    friend class CRenderPass;

public:

    //! Constructeur
    CMaterial(const QString& name = "Material");

    //! Destructeur
    virtual ~CMaterial();

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IMaterialListener
    {
        friend class CMaterial;

    protected:
        virtual void onDelete(CMaterial* pMaterial) = 0;
        virtual void onUpdate(CMaterial* pMaterial) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMaterialListener* pListener);

    //! Libére un listener
    void unregisterListener(IMaterialListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Multi-Pass
    //-------------------------------------------------------------------------------------------------

    //! Retourne la liste des passes dans le cas d'un rendu multi-pass
    const QList<CRenderPass*>& renderingPassList() const { return m_RenderingPassList; }

    //! Crée une passe de rendu
    CRenderPass* createRenderPass(
            EnumTarget eTarget,
            const QSize& size = QSize(1024, 1024),
            const CRenderStates& states = CRenderStates());

    //! Supprime une passe de rendu
    void removeRenderingPass(int iIndex);

    //! Supprime une passe de rendu
    void removeRenderingPass(CRenderPass* pPass);

    //! Retourne une passe de rendu
    CRenderPass* getRenderPass(int iIndex);

    //! Retourne une passe de rendu
    const CRenderPass* getRenderPass(int iIndex) const;

    //! Retourne l'ensemble des passes de rendu
    const QList<CRenderPass*> getRenderPasses() const;

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Ajoute une texture
    void addTexture(ATexture* pTexture, EnumMaterialParameter eMaterialParameter);

    //! Ajoute une texture
    void addTexture(const QString& textureName, EnumMaterialParameter eMaterialParameter);

    //! Supprime une texture
    void removeTexture(const QString& textureName);

    //! Définit la couleur ambiante
    void setAmbientColor(const QVector4D& color);

    //! Définit la couleur ambiante
    void setAmbientColor(real r, real g, real b, real a = 1.0);

    //! Définit la couleur diffuse
    void setDiffuseColor(const QVector4D& color);

    //! Définit la couleur diffuse
    void setDiffuseColor(real r, real g, real b, real a = 1.0);

    //! Définit la couleur spéculaire
    void setSpecularColor(const QVector4D& color);

    //! Définit la couleur spéculaire
    void setSpecularColor(real r, real g, real b, real a = 1.0);

    //! Définit la couleur du masque alpha
    void setAlphaMaskColor(const QVector4D& color);

    //! Définit la couleur du masque alpha
    void setAlphaMaskColor(real r, real g, real b, real a = 1.0);

    //! Définit la fonction de comparaison alpha
    void setAlphaMaskFunc(EnumAlphaMaskFunc eAlphaMaskFunc);

    //! Définit le facteur de brillance
    void setShininessFactor(real shininess);

    //! Définit l'opacité du matériau
    void setOpacity(real dOpacity);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Material"; }

    //! Retourne le conteneur des textures
    QList<CTextureParam>& getTextureParams() { return m_Textures; }

    //! Retourne le conteneur des textures
    const QList<CTextureParam>& getTextureParams() const { return m_Textures; }

    //! Retourne la couleur ambiante
    QVector4D getAmbientColor() const;

    //! Retourne la couleur diffuse
    QVector4D getDiffuseColor() const;

    //! Retourne la couleur spéculaire
    QVector4D getSpecularColor() const;

    //! Retourne la couleur du masque alpha
    QVector4D getAlphaMaskColor() const;

    //! Retourne la fonction de comparaison alpha
    EnumAlphaMaskFunc getAlphaMaskFunc() const;

    //! Retourne le facteur de brillance
    real getShininessFactor() const;

    //! Retourne vrai si le matériau est texturé
    bool isTextured() const;

    //! Retourne l'opacité du materiau
    real getOpacity() const;

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[Material \n";
        result += AEntity::toString();
        result += QString("Ambient color: %1\n").arg(StringHelper::toQString(m_AmbientColor));
        result += QString("Diffuse color: %1\n").arg(StringHelper::toQString(m_DiffuseColor));
        result += QString("Specular color: %1\n").arg(StringHelper::toQString(m_SpecularColor));
        result += QString("Alpha Mask color: %1\n").arg(StringHelper::toQString(m_AlphaMaskColor));
        result += QString("Shininess Factor: %1\n").arg(m_dShininessFactor);
        result += QString("Opacity: %1\n").arg(m_dOpacity);
        result += QString("Alpha Mask Func: %1\n").arg(CGeometryGlobal::stringFromAlphaMaskFunc(m_eAlphaMaskFunc));

        foreach (const CTextureParam& texture, m_Textures)
        {
            result += QString("Texture : ID %1 Parameter %2\n")
                    .arg(texture.getTextureName())
                    .arg(CGeometryGlobal::stringFromMaterialParameter(texture.getMaterialParameter()));

            if (ATexture* pTexture = CTextureManager::getInstance()->getTextureByName(texture.getTextureName()))
            {
                result += pTexture->toString();
            }
        }

        foreach (CRenderPass* pPass, m_RenderingPassList)
        {
            result += pPass->toString();
        }

        result += "]\n";
        return result;
    }

protected:

    //! Transmet le message de mise à jour
    virtual void notifyUpdate();

    //! Transmet le message de suppression
    virtual void notifyDelete();

    //! Liste des listeners
    QSet<IMaterialListener*> m_MaterialListeners;

private:

    //! Conteneur des ID de texture
    QList<CTextureParam> m_Textures;

    //! Couleur ambiante
    QVector4D m_AmbientColor;

    //! Couleur diffuse
    QVector4D m_DiffuseColor;

    //! Couleur spéculaire
    QVector4D m_SpecularColor;

    //! Couleur du masque alpha
    QVector4D m_AlphaMaskColor;

    //! Facteur de brillance
    real m_dShininessFactor;

    //! Opacité du materiau
    real m_dOpacity;

    //! Fonction de comparaison alpha
    EnumAlphaMaskFunc m_eAlphaMaskFunc;

    //! Liste des passes d'un rendu
    QList<CRenderPass*> m_RenderingPassList;
};


#endif //CMATERIAL_H

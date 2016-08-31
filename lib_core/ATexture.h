#ifndef ATEXTURE_H
#define ATEXTURE_H

#include "AEntity.h"


class LIB_CORE_SHARED_EXPORT ATexture : public AEntity
{

public:

    ATexture(const QString& name, EnumTextureType eType, int iTextureUnit)
        : AEntity(name)
        , m_eType(eType)
        , m_iTextureUnit(iTextureUnit)
        , m_bIsValid(false)
    {
    }

    //! Destructeur
    virtual ~ATexture() {}

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit le type de cible (2D, 3D, Cube...)
    void setType(EnumTextureType eType)					{ m_eType = eType; notifyUpdate(); }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le type de cible (2D, 3D, Cube...)
    EnumTextureType getType() const						{ return m_eType; }

    //! Retourne le numéro d'unité de texture
    int getTextureUnit() const							{ return m_iTextureUnit; }

    //! Retourne vrai si la texture est valide
    bool isValid() const								{ return m_bIsValid; }


    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class ITextureListener
    {
        friend class ATexture;

    protected:
        virtual void onDelete(ATexture* pTexture) = 0;
        virtual void onUpdate(ATexture* pTexture) = 0;
    };

    //! Enregistre un listener
    void registerListener(ITextureListener* pListener)
    {
        m_TextureListeners.insert(pListener);
    }

    //! Libére un listener
    void unregisterListener(ITextureListener* pListener)
    {
        m_TextureListeners.remove(pListener);
    }

protected:

    //! Transmet le message de mise à jour
    virtual void notifyUpdate()
    {
        if (m_bNotificationsEnabled)
        {
            foreach (ITextureListener* pListener, m_TextureListeners)
            {
                pListener->onUpdate(this);
            }

            AEntity::notifyUpdate();
        }
    }

    //! Transmet le message de suppression
    virtual void notifyDelete()
    {
        if (m_bNotificationsEnabled)
        {
            foreach (ITextureListener* pListener, m_TextureListeners)
            {
                pListener->onDelete(this);
            }

            AEntity::notifyDelete();
        }
    }

    //! Liste des listeners
    QSet<ITextureListener*> m_TextureListeners;

    //! Type de cible (2D, 3D, Cube...)
    EnumTextureType m_eType;

    //! Numéro d'unité de texture
    int m_iTextureUnit;

    //! Vrai si la texture est valide
    bool m_bIsValid;
};


#endif // ATEXTURE_H

#ifndef ARENDERABLEITEM_H
#define ARENDERABLEITEM_H

// Lib
#include "lib_core_global.h"


// Foundations
#include "CGeometryGlobal.h"
#include "ASceneNodeItem.h"
#include "CMaterialManager.h"
#include "CRenderStates.h"

class ARenderer;
class CSceneManager;


class LIB_CORE_SHARED_EXPORT ARenderableItem : public ASceneNodeItem
{
public:

    //! Constructeur
    ARenderableItem(CSceneManager* pSceneManager, const QString& name);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IRenderableItemListener
    {
        friend class ARenderableItem;

    protected:
        virtual void onMaterialChanged(ARenderableItem* pItem) = 0;
    };

    //! Enregistre un listener
    void registerListener(IRenderableItemListener* pListener)
    {
        m_RenderableItemListeners.insert(pListener);
    }

    //! Lib�re un listener
    void unregisterListener(IRenderableItemListener* pListener)
    {
        m_RenderableItemListeners.remove(pListener);
    }

    //! Pr�vient les listeners que le mat�riau a chang�
    void notifyMaterialChanged()
    {
        foreach (IRenderableItemListener* pListener, m_RenderableItemListeners)
        {
            pListener->onMaterialChanged(this);
        }
    }

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! D�finit le materiau
    void setMaterialName(const QString& materialName);

    //! D�finit les param�tres de rendu
    void setRenderStates(const CRenderStates& renderStates)                     { m_RenderStates = renderStates; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne vrai si peut �tre rendu
    virtual bool isRenderable() const = 0;

    //! Retourne vrai si poss�de un squelette d'animation
    virtual bool hasSkeleton() const { return false; }

    //! Retourne vrai si recoit les ombres
    virtual bool isShadowReceiver() const { return true; }

    //! Retourne vrai si produit des ombres
    virtual bool isShadowCaster() const { return true; }

    //! Retourne un conteneur contenant l'ensemble des matrices d'offset des bones
    virtual QVector<QMatrix4x4> getBoneTransformations() const { return QVector<QMatrix4x4>(); }

    //! Retourne l'ID du materiau
    const QString& getMaterialName() const;

    //! Retourne true si a un mat�riau
    bool hasMaterial() const;

    //! Retourne les param�tres de rendu
    const CRenderStates& getRenderStates() const                                { return m_RenderStates; }

    //! Effectue le rendu
    void doRender(ARenderer* pRenderer)
    {
        if (!m_bIsRenderingInit)
        {
            m_bIsRenderingInit = true;
            initRendering(pRenderer);
        }

        preRender();
        render(pRenderer);
        postRender();
    }

    //! Cr�e une string contenant les propri�t�s de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[ARenderableItem \n";
        result += ASceneNodeItem::toString();
        result += QString("Rendering Init: %1\n").arg(m_bIsRenderingInit);
        result += QString("Material Name: %1\n").arg(m_MaterialName);
        result += "]\n";
        return result;
    }

protected:

    //! Liste des listeners
    QSet<IRenderableItemListener*> m_RenderableItemListeners;

    //! Fonction appel�e avant le premier appel � render()
    virtual void initRendering(ARenderer*) {}

    //! Fonction appel�e avant le rendu
    virtual void preRender() {}

    //! Fonction appel�e apr�s le rendu
    virtual void postRender() {}

    //! D�finit le rendu � r�aliser
    virtual void render(ARenderer* pRenderer) = 0;

    //! Param�tres de rendu
    CRenderStates m_RenderStates;

    //! Vrai si l'initialisation pre-rendu a �t� effectu�e
    bool m_bIsRenderingInit;

    //! Identifiant du materiau
    QString m_MaterialName;
};


#endif // ARENDERABLEITEM_H

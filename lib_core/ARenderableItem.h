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

    //! Libére un listener
    void unregisterListener(IRenderableItemListener* pListener)
    {
        m_RenderableItemListeners.remove(pListener);
    }

    //! Prévient les listeners que le matériau a changé
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

    //! Définit le materiau
    void setMaterialName(const QString& materialName);

    //! Définit les paramétres de rendu
    void setRenderStates(const CRenderStates& renderStates)                     { m_RenderStates = renderStates; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne vrai si peut être rendu
    virtual bool isRenderable() const = 0;

    //! Retourne vrai si possède un squelette d'animation
    virtual bool hasSkeleton() const { return false; }

    //! Retourne vrai si recoit les ombres
    virtual bool isShadowReceiver() const { return true; }

    //! Retourne vrai si produit des ombres
    virtual bool isShadowCaster() const { return true; }

    //! Retourne un conteneur contenant l'ensemble des matrices d'offset des bones
    virtual QVector<QMatrix4x4> getBoneTransformations() const { return QVector<QMatrix4x4>(); }

    //! Retourne l'ID du materiau
    const QString& getMaterialName() const;

    //! Retourne true si a un matériau
    bool hasMaterial() const;

    //! Retourne les paramétres de rendu
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

    //! Crée une string contenant les propriétés de l'objet
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

    //! Fonction appelée avant le premier appel à render()
    virtual void initRendering(ARenderer*) {}

    //! Fonction appelée avant le rendu
    virtual void preRender() {}

    //! Fonction appelée après le rendu
    virtual void postRender() {}

    //! Définit le rendu à réaliser
    virtual void render(ARenderer* pRenderer) = 0;

    //! Paramètres de rendu
    CRenderStates m_RenderStates;

    //! Vrai si l'initialisation pre-rendu a été effectuée
    bool m_bIsRenderingInit;

    //! Identifiant du materiau
    QString m_MaterialName;
};


#endif // ARENDERABLEITEM_H

#ifndef CANIMATION_H
#define CANIMATION_H

// Lib
#include "lib_core_global.h"

// Qt
#include <QString>
#include <QList>

// Foundations
#include "CMath.h"
#include "AEntity.h"
#include "CSceneNodeAnimation.h"
#include "CMeshAnimation.h"

static const real DefaultTicksPerSecond = 25.;

class LIB_CORE_SHARED_EXPORT CAnimation : public AEntity
{
public:

    //! Diffrents état de l'animation
    enum EnumStateAnimation
    {
        eStarted,
        ePaused,
        eStopped
    };

    //! Constructeur
    CAnimation(const QString& name);

    //! Destructeur
    virtual ~CAnimation();

    //! Démarre l'animation
    void start();

    //! Met l'animation en pause
    void pause();

    //! Arrête l'animation
    void stop();

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IAnimationListener
    {
        friend class CAnimation;

    protected:
        //! Prévient du démarrage d'une animation
        virtual void onStart(CAnimation* pAnimation) = 0;

        //! Prévient de la mise en pause d'une animation
        virtual void onPause(CAnimation* pAnimation) = 0;

        //! Prévient de l'arrêt d'une animation
        virtual void onStop(CAnimation* pAnimation) = 0;
    };

    //! Enregistre un listener
    void registerListener(IAnimationListener* pListener);

    //! Libére un listener
    void unregisterListener(IAnimationListener* pListener);


    //-------------------------------------------------------------------------------------------------
    //
    //-------------------------------------------------------------------------------------------------

    //! Crée une animation de noeud
    CSceneNodeAnimation* createNodeAnimation(const QString& nodeName, const QString& name = "NodeAnimation");

    //! Crée une animation de mesh
    CMeshAnimation* createMeshAnimation(const QString& meshName, const QString& name = "MeshAnimation");

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit la durée de l'animation
    void setDuration(real dDuration) { m_dDuration = dDuration; }

    //! Définit le nombre d'impulsions par seconde
    void setTicksPerSecond(real dTicksPerSecond) { m_dTicksPerSecond = dTicksPerSecond; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Animation"; }

    //! Retourne l'état de l'animation
    EnumStateAnimation getState() const { return m_eState; }

    //! Retourne la durée de l'animation
    real getDuration() const { return m_dDuration; }

    //! Retourne le nombre d'impulsions par seconde
    real getTicksPerSecond() const { return m_dTicksPerSecond; }

    //! Retourne les animations des noeuds
    const QList<CSceneNodeAnimation*>& getNodeAnimations() const { return m_NodeAnimations; }

    //! Retourne les animations des mehs
    const QList<CMeshAnimation*>& getMeshAnimations() const { return m_MeshAnimations; }

protected:

    //! Liste des listeners
    QSet<IAnimationListener*> m_AnimationListeners;

    //! Notifie les listeners que l'animation démarre
    void notifyStart();

    //! Notifie les listeners que l'animation est mise en pause
    void notifyPause();

    //! Notifie les listeners que l'animation est arrêtée
    void notifyStop();

private:

    //! Etat de l'animation
    EnumStateAnimation m_eState;

    //! Durée de l'animation
    real m_dDuration;

    //! Nombre d'impulsions par seconde
    real m_dTicksPerSecond;

    //! Animations des noeuds
    QList<CSceneNodeAnimation*> m_NodeAnimations;

    //! Animations des meshs
    QList<CMeshAnimation*> m_MeshAnimations;
};

#endif // CANIMATION_H

#ifndef CANIMATIONMANAGER_H
#define CANIMATIONMANAGER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CSceneManager.h"
#include "CAnimationHelper.h"

// Qt
#include <QObject>
#include <QTimer>
#include <QTime>


class LIB_CORE_SHARED_EXPORT CAnimationManager : public QObject, public CSceneManager::IAnimationListener
{
    Q_OBJECT

public:

    //! Constructeur
    CAnimationManager(CSceneManager* pSceneManager);

    //! Destructeur
    virtual ~CAnimationManager();

    //! Joue les animations
    void run();

    //! Arr�te les animations
    void stop();

    //! D�finit le temps entre deux mise � jour des animations
    void setDT(unsigned int iDT);

    //! Retourne le temps entre deux mise � jour des animations
    unsigned int getDT() const;

    class IAnimationManagerListener
    {
        friend class CAnimationManager;

    protected:
        //! Pr�vient du d�marrage d'une animation
        virtual void onUpdateAnimation() = 0;
    };

    //! Enregistre un listener
    void registerListener(IAnimationManagerListener* pListener);

    //! Lib�re un listener
    void unregisterListener(IAnimationManagerListener* pListener);

protected:

    //! Impl�mentation de CSceneManager::IAnimationListener
    virtual void onStartAnimation(CAnimation* pAnimation);

    //! Impl�mentation de CSceneManager::IAnimationListener
    virtual void onPauseAnimation(CAnimation* pAnimation);

    //! Impl�mentation de CSceneManager::IAnimationListener
    virtual void onStopAnimation(CAnimation* pAnimation);

    //! Liste des listeners
    QSet<IAnimationManagerListener*> m_AnimationManagerListeners;

private slots:

    void onTimerTimeout();

private:

    //! Vrai si est d�marr�
    bool m_bRun;

    //! Pointeur sur le gestionnaire de sc�nes
    CSceneManager* m_pSceneManager;

    //! Timer de mise � jur des animations
    QTimer m_Timer;

    //! Temps
    QTime m_Time;

    //! Temps entre deux mise � jour des animations
    unsigned int m_iDT;

    //! Temps par d�faut entre deux mise � jour des animations
    static const unsigned int s_iDefaultDT = 1000 / 60;

    struct SAnimationProperties
    {
        real dStartTime;
        real dPauseTime;
        bool bIsPausing;
        bool bIsRunning;
    };

    //! Conteneur des animations
    QHash<CAnimation*, SAnimationProperties> m_AnimationsProperties;
    QList<CAnimation*> m_Animations;

    CAnimationHelper::Interpolator<CVectorKey> m_vIpl;
    CAnimationHelper::Interpolator<CQuaternionKey> m_qIpl;
    CAnimationHelper::Interpolator<CMeshKey> m_mIpl;

    //! Notifie les listeners qu'au moins une animation a �t� mise � jour
    void notifyUpdate();
};

#endif // CANIMATIONMANAGER_H

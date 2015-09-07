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

	//! Arrête les animations
	void stop();

	//! Définit le temps entre deux mise à jour des animations
	void setDT(unsigned int iDT);

	//! Retourne le temps entre deux mise à jour des animations
	unsigned int getDT() const;

    class IAnimationManagerListener
	{
		friend class CAnimationManager;

	protected:
		//! Prévient du démarrage d'une animation
		virtual void onUpdateAnimation() = 0;
	};

	//! Enregistre un listener
    void registerListener(IAnimationManagerListener* pListener);

	//! Libére un listener
    void unregisterListener(IAnimationManagerListener* pListener);

protected:

	//! Implémentation de CSceneManager::IAnimationListener
	virtual void onStartAnimation(CAnimation* pAnimation);

	//! Implémentation de CSceneManager::IAnimationListener
	virtual void onPauseAnimation(CAnimation* pAnimation);

	//! Implémentation de CSceneManager::IAnimationListener
	virtual void onStopAnimation(CAnimation* pAnimation);

    //! Liste des listeners
    QSet<IAnimationManagerListener*> m_AnimationManagerListeners;

private slots:

	void onTimerTimeout();

private:
	
	//! Vrai si est démarré
	bool m_bRun;

	//! Pointeur sur le gestionnaire de scénes
	CSceneManager* m_pSceneManager;

	//! Timer de mise à jur des animations
	QTimer m_Timer;

	//! Temps
	QTime m_Time;

	//! Temps entre deux mise à jour des animations
	unsigned int m_iDT;

	//! Temps par défaut entre deux mise à jour des animations
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

	//! Notifie les listeners qu'au moins une animation a été mise à jour
	void notifyUpdate();
};

#endif // CANIMATIONMANAGER_H

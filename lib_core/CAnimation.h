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

	//! Diffrents �tat de l'animation
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

	//! D�marre l'animation
	void start();

	//! Met l'animation en pause
	void pause();

	//! Arr�te l'animation
	void stop();

	//-------------------------------------------------------------------------------------------------
	// Listeners
	//-------------------------------------------------------------------------------------------------

    class IAnimationListener
	{
		friend class CAnimation;

	protected:
		//! Pr�vient du d�marrage d'une animation
		virtual void onStart(CAnimation* pAnimation) = 0;

		//! Pr�vient de la mise en pause d'une animation
		virtual void onPause(CAnimation* pAnimation) = 0;

		//! Pr�vient de l'arr�t d'une animation
		virtual void onStop(CAnimation* pAnimation) = 0;
	};

	//! Enregistre un listener
    void registerListener(IAnimationListener* pListener);

	//! Lib�re un listener
    void unregisterListener(IAnimationListener* pListener);


	//-------------------------------------------------------------------------------------------------
	// 
	//-------------------------------------------------------------------------------------------------

	//! Cr�e une animation de noeud
	CSceneNodeAnimation* createNodeAnimation(const QString& nodeName, const QString& name = "NodeAnimation");

	//! Cr�e une animation de mesh
    CMeshAnimation* createMeshAnimation(const QString& meshName, const QString& name = "MeshAnimation");

	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! D�finit la dur�e de l'animation
	void setDuration(real dDuration) { m_dDuration = dDuration; }

	//! D�finit le nombre d'impulsions par seconde
	void setTicksPerSecond(real dTicksPerSecond) { m_dTicksPerSecond = dTicksPerSecond; }

	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne le nom du type
	virtual QString getTypeName() const { return "Animation"; }

	//! Retourne l'�tat de l'animation
	EnumStateAnimation getState() const { return m_eState; }

	//! Retourne la dur�e de l'animation
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

    //! Notifie les listeners que l'animation d�marre
    void notifyStart();

    //! Notifie les listeners que l'animation est mise en pause
    void notifyPause();

    //! Notifie les listeners que l'animation est arr�t�e
    void notifyStop();

private:

	//! Etat de l'animation
	EnumStateAnimation m_eState;

	//! Dur�e de l'animation
	real m_dDuration;

	//! Nombre d'impulsions par seconde
	real m_dTicksPerSecond;
	
	//! Animations des noeuds
	QList<CSceneNodeAnimation*> m_NodeAnimations;

	//! Animations des meshs
    QList<CMeshAnimation*> m_MeshAnimations;	
};

#endif // CANIMATION_H

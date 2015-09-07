#include "CAnimationManager.h"

//-----------------------------------------------------------------------------------------
CAnimationManager::CAnimationManager(CSceneManager* pSceneManager)
: m_bRun(false)
, m_pSceneManager(pSceneManager)
, m_iDT(s_iDefaultDT)
{
	m_pSceneManager->registerAnimationListener(this);
	m_Time.start();
	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
}

//-----------------------------------------------------------------------------------------
CAnimationManager::~CAnimationManager()
{
	if (m_pSceneManager)
	{
		m_pSceneManager->unregisterAnimationListener(this);
	}
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::registerListener(IAnimationManagerListener* pListener)
{
    m_AnimationManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::unregisterListener(IAnimationManagerListener* pListener)
{
    m_AnimationManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::run()
{
	m_bRun = true;
	m_Timer.start(m_iDT);
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::stop()
{
	m_bRun = false;
	m_Timer.stop();
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::setDT(unsigned int iDT)
{
	m_iDT = iDT;
	m_Timer.stop();
	m_Timer.start(m_iDT);
}

//-----------------------------------------------------------------------------------------
unsigned int CAnimationManager::getDT() const
{
	return m_iDT;
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::onTimerTimeout()
{
    if (m_bRun)
    {
		bool bNeedUpdate = false;

        foreach (CAnimation* pAnimation, m_Animations)
        {
			real dTicksPerSecond = pAnimation->getTicksPerSecond() != 0 ? pAnimation->getTicksPerSecond() : DefaultTicksPerSecond;

            const QList<CSceneNodeAnimation*>& nodeChannels = pAnimation->getNodeAnimations();

            foreach (CSceneNodeAnimation* pNodeAnimation, nodeChannels)
            {
				real dNodeAnimationTime = pNodeAnimation->getAnimationTime();

                if (dNodeAnimationTime > 0.)
                {
                    if (CSceneNode* pNode = m_pSceneManager->findNode(pNodeAnimation->getNodeName()))
                    {
                        real dTotalAnimationTime = dNodeAnimationTime / dTicksPerSecond * 1000.;
                        real dGlobalTime = (real) m_Time.elapsed();

                        real dCurrentAnimationTime;

                        SAnimationProperties& properties = m_AnimationsProperties[pAnimation];

                        if (pNodeAnimation->getAnimationBehavior() == eAnimationBehaviourRepeat)
                        {
                            dCurrentAnimationTime = fmod(dGlobalTime - properties.dStartTime, dTotalAnimationTime);
                        }
                        else
                        {
                            dCurrentAnimationTime = dGlobalTime - properties.dStartTime;
                        }

                        if (dCurrentAnimationTime < dTotalAnimationTime)
						{
							pNode->resetTransformation();

							double t = (dCurrentAnimationTime / 1000.) * dTicksPerSecond;

							//---------------------------------------------------------
							// Positions Interpolation
							//---------------------------------------------------------
							QPair<CVectorKey, CVectorKey> posKeys = pNodeAnimation->getPositionKeys(t);
							QVector3D position;

							if (posKeys.first == posKeys.second)
							{
								position = posKeys.first.getValue();
							}
							else
							{
								double t1 = posKeys.first.getTime();
								double t2 = posKeys.second.getTime();

								m_vIpl(position, posKeys.first, posKeys.second, (t - t1)/(t2 - t1));
							}
							
							//---------------------------------------------------------
							// Rotations Interpolation
							//---------------------------------------------------------
							QPair<CQuaternionKey, CQuaternionKey> rotKeys = pNodeAnimation->getRotationKeys(t);
							QQuaternion rotation;

							if (rotKeys.first == rotKeys.second)
							{
								rotation = rotKeys.first.getValue();
							}
							else
							{
								double t1 = rotKeys.first.getTime();
								double t2 = rotKeys.second.getTime();
								m_qIpl(rotation, rotKeys.first, rotKeys.second, (t - t1)/(t2 - t1));
							}

							//---------------------------------------------------------
							// Scaling Interpolation
							//---------------------------------------------------------
							QPair<CVectorKey, CVectorKey> scaKeys = pNodeAnimation->getScalingKeys(t);
							QVector3D scaling;

							if (scaKeys.first == scaKeys.second)
							{
								scaling = scaKeys.first.getValue();
							}
							else
							{
								double t1 = scaKeys.first.getTime();
								double t2 = scaKeys.second.getTime();
								m_vIpl(scaling, scaKeys.first, scaKeys.second, (t - t1)/(t2 - t1));
							}
						
							pNode->translate(position);
							pNode->rotate(rotation);
							pNode->scale(scaling);						

							bNeedUpdate = true;
						}
                    }
                }
			}
		}

		if (bNeedUpdate)
		{
			notifyUpdate();
		}
	}
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::onStartAnimation(CAnimation* pAnimation)
{
    if (!m_AnimationsProperties.contains(pAnimation))
    {
        SAnimationProperties properties;
        properties.bIsPausing = false;
        properties.bIsRunning = true;
        properties.dStartTime = m_Time.elapsed();
        properties.dPauseTime = m_Time.elapsed();
        m_Animations << pAnimation;
        m_AnimationsProperties[pAnimation] = properties;
    }
    else
    {
        SAnimationProperties& properties = m_AnimationsProperties[pAnimation];
        if (properties.bIsPausing)
        {
            properties.dStartTime = m_Time.elapsed() - properties.dPauseTime;
            properties.bIsPausing = false;
        }
        properties.bIsRunning = true;
    }
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::onPauseAnimation(CAnimation* pAnimation)
{
    SAnimationProperties& properties = m_AnimationsProperties[pAnimation];
    if (properties.bIsRunning)
    {
        properties.bIsPausing = true;
        properties.dPauseTime = m_Time.elapsed();
    }
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::onStopAnimation(CAnimation* pAnimation)
{
    m_Animations.removeOne(pAnimation);
    m_AnimationsProperties.remove(pAnimation);
}

//-----------------------------------------------------------------------------------------
void CAnimationManager::notifyUpdate()
{
    foreach (IAnimationManagerListener* pListener, m_AnimationManagerListeners)
	{
		pListener->onUpdateAnimation();
	}
}

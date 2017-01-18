#include "CAnimation.h"

//--------------------------------------------------------------------------
CAnimation::CAnimation(const QString& name)
    : AEntity(name)
    , m_eState(eStopped)
    , m_dDuration(-1.)
    , m_dTicksPerSecond(DefaultTicksPerSecond)
{
}

//--------------------------------------------------------------------------
CAnimation::~CAnimation()
{
    for (CSceneNodeAnimation* pNodeAnimation : m_NodeAnimations)
    {
        m_NodeAnimations.removeOne(pNodeAnimation);
        delete pNodeAnimation;
    }

    for (CMeshAnimation* pMeshAnimation : m_MeshAnimations)
    {
        m_MeshAnimations.removeOne(pMeshAnimation);
        delete pMeshAnimation;
    }
}

//--------------------------------------------------------------------------
void CAnimation::start()
{
    notifyStart();
}

//--------------------------------------------------------------------------
void CAnimation::pause()
{
    notifyPause();
}

//--------------------------------------------------------------------------
void CAnimation::stop()
{
    notifyStop();
}

//--------------------------------------------------------------------------
void CAnimation::registerListener(IAnimationListener* pListener)
{
    m_AnimationListeners.insert(pListener);
}

//--------------------------------------------------------------------------
void CAnimation::unregisterListener(IAnimationListener* pListener)
{
    m_AnimationListeners.remove(pListener);
}

//--------------------------------------------------------------------------
CSceneNodeAnimation* CAnimation::createNodeAnimation(const QString& nodeName, const QString& name /*= "NodeAnimation"*/)
{
    CSceneNodeAnimation* pNodeAnimation = new CSceneNodeAnimation(nodeName, name);
    m_NodeAnimations.append(pNodeAnimation);
    return pNodeAnimation;
}

//--------------------------------------------------------------------------
CMeshAnimation* CAnimation::createMeshAnimation(const QString& meshName, const QString& name /*= "MeshAnimation"*/)
{
    CMeshAnimation* pMeshAnimation = new CMeshAnimation(meshName, name);
    m_MeshAnimations.append(pMeshAnimation);
    return pMeshAnimation;
}

//--------------------------------------------------------------------------
void CAnimation::notifyStart()
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onStart(this);
    }
}

//--------------------------------------------------------------------------
void CAnimation::notifyPause()
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onPause(this);
    }
}

//--------------------------------------------------------------------------
void CAnimation::notifyStop()
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onStop(this);
    }
}

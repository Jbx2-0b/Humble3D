#include "Humble3DApp.h"
#include "CLogManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"
#include "CRenderBufferManager.h"
#include "CMeshManager.h"
#include "CSceneManager.h"
#include "CAnimationManager.h"
#include "CWidget3D.h"
#include "CGraphicsView3D.h"
#include "CSoftwareView3D.h"
#include "CDebugManager.h"

//-----------------------------------------------------------------------------------------
Humble3DApp::Humble3DApp()
{
    LogManager.addMessage(eINFO, "GUI Thread ID: " + QString::number((qlonglong)QThread::currentThreadId()));

    m_pSceneManager = new CSceneManager();
    m_pAnimationManager = new CAnimationManager(m_pSceneManager);
    CDebugManager::createInstance(m_pSceneManager);

#ifdef EMBEDDED_TARGET
    CShaderManager::getInstance().createShader(
                "phong",
                "://shaders/phongES.vertex.glsl",
                "",
                "://shaders/phongES.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "toon",
                "://shaders/toonES.vertex.glsl",
                "",
                "://shaders/toonES.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "color",
                "://shaders/colorES.vertex.glsl",
                "",
                "://shaders/colorES.fragment.glsl");
#else // DESKTOP_TARGET
    CShaderManager::getInstance().createShader(
                "phong",
                "://shaders/phong.vertex.glsl",
                "",
                "://shaders/phong.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "toon",
                "://shaders/toon.vertex.glsl",
                "",
                "://shaders/toon.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "color",
                "://shaders/color.vertex.glsl",
                "",
                "://shaders/color.fragment.glsl");
#endif
}

//-----------------------------------------------------------------------------------------
Humble3DApp::~Humble3DApp()
{
    foreach (AView* pView, m_Views)
    {
        delete pView;
    }

    CDebugManager::getInstance()->killInstance();

    delete m_pAnimationManager;
    delete m_pSceneManager;
}

//-----------------------------------------------------------------------------------------
CSceneManager* Humble3DApp::getSceneManager()
{
    return m_pSceneManager;
}

//-----------------------------------------------------------------------------------------
CAnimationManager* Humble3DApp::getAnimationManager()
{
    return m_pAnimationManager;
}

//-----------------------------------------------------------------------------------------
CWidget3D* Humble3DApp::createWidget3D(CCamera* pCamera, QWidget* parent /* = 0*/)
{
    CWidget3D* pWidget3D = new CWidget3D(m_pSceneManager, pCamera, parent);
    m_Views.append(pWidget3D);
    pWidget3D->show();
    return pWidget3D;
}

//-----------------------------------------------------------------------------------------
CGraphicsView3D* Humble3DApp::createGraphicsView3D(CCamera* pCamera, QWidget* parent /* = 0*/)
{
    CGraphicsView3D* pGraphicsView3D = new CGraphicsView3D(m_pSceneManager, pCamera, parent);
    m_Views.append(pGraphicsView3D);
    pGraphicsView3D->show();
    return pGraphicsView3D;
}

//-----------------------------------------------------------------------------------------
CSoftwareView3D* Humble3DApp::createSoftwareView3D(CCamera* pCamera, QWidget* parent /* = 0*/)
{
    CSoftwareView3D* pSoftwareView3D = new CSoftwareView3D(m_pSceneManager, pCamera, parent);
    m_Views.append(pSoftwareView3D);
    pSoftwareView3D->show();
    return pSoftwareView3D;
}

//-----------------------------------------------------------------------------------------
const QList<AView*>& Humble3DApp::getViews() const
{
    return m_Views;
}

//-----------------------------------------------------------------------------------------
QList<AView*>& Humble3DApp::getViews()
{
    return m_Views;
}

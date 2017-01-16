#include "Humble3DQmlApp.h"
#include "CLogManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"
#include "CRenderBufferManager.h"
#include "CMeshManager.h"
#include "CSceneManager.h"
#include "CAnimationManager.h"
#include "CDebugManager.h"


Humble3DQmlApp* Humble3DQmlApp::s_instance = nullptr;

//-----------------------------------------------------------------------------------------
Humble3DQmlApp::Humble3DQmlApp()
{
    Q_ASSERT(s_instance == nullptr);
    s_instance = this;

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
Humble3DQmlApp::~Humble3DQmlApp()
{
    CDebugManager::getInstance()->killInstance();

    delete m_pAnimationManager;
    delete m_pSceneManager;

    s_instance = nullptr;
}

//-----------------------------------------------------------------------------------------
Humble3DQmlApp* Humble3DQmlApp::instance()
{
    return s_instance;
}

//-----------------------------------------------------------------------------------------
CSceneManager* Humble3DQmlApp::getSceneManager()
{
    return m_pSceneManager;
}

//-----------------------------------------------------------------------------------------
CAnimationManager* Humble3DQmlApp::getAnimationManager()
{
    return m_pAnimationManager;
}


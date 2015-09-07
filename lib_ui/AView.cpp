#include "AView.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------------------------
AView::AView(CSceneManager* pSceneManager)
    : m_pRenderer(0)
    , m_pSceneManager(pSceneManager)
    , m_BackgroundColor(Qt::black)
    , m_MouseMoveFilter(this, 40)
{
}

//-----------------------------------------------------------------------------------------
AView::~AView()
{
    delete m_pRenderer;
}

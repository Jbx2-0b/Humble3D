#include "CGLContext.h"

CGLContext::CGLContext()
    : m_pContext(0)
    , m_pSurface(0)
{
}

void CGLContext::init()
{
    m_pContext = QOpenGLContext::currentContext();
    m_pSurface = m_pContext->surface();
}

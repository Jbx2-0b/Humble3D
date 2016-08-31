#ifndef CGLCONTEXT_H
#define CGLCONTEXT_H

#include "CGLGlobal.h"
#include <QOpenGLContext>
#include <QDebug>

class CGLContext
{
public:
    CGLContext();

    void init();

    bool isValid() const { return m_pContext && m_pSurface; }

    CGLContext* createSharedContext(QThread* pThread = 0)
    {
        CGLContext* pSharedContext = new CGLContext();
        pSharedContext->m_pContext = new QOpenGLContext();
        pSharedContext->m_pContext->setFormat(m_pContext->format());
        pSharedContext->m_pSurface = m_pContext->surface();
        pSharedContext->m_pContext->setShareContext(m_pContext);
        pSharedContext->m_pContext->create();

        if (pThread)
        {
            pSharedContext->m_pContext->moveToThread(pThread);
        }
        return pSharedContext;
    }


    void makeCurrent()
    {
        m_pContext->makeCurrent(m_pSurface);
    }

    void doneCurrent()
    {
        m_pContext->doneCurrent();
    }

private:
    QOpenGLContext* m_pContext;
    QSurface* m_pSurface;
};

#endif // CGLCONTEXT_H

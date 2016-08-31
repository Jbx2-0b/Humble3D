#ifndef CGLFRAMEBUFFER_H
#define CGLFRAMEBUFFER_H

// Fundations
#include "CGLGlobal.h"

// Qt
#include <QDebug>

/**
    The reason that a FBO has multiple color attachement points is to allow to render
    the color buffer to multiple destinations at the same time.
*/
class CGLFrameBuffer : protected CGLFunctions
{
public:

    CGLFrameBuffer(int iWidth, int iHeight)
        : m_iWidth(iWidth)
        , m_iHeight(iHeight)
    {
        initializeOpenGLFunctions();
        glGenFramebuffers(1, &m_iID);
    }

    ~CGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_iID);
    }

    void attachTexture(int iTextureID, EnumAttachment eAttachment)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_iID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, CGLHelper::toGLType(eAttachment), GL_TEXTURE_2D, iTextureID, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void attachRenderbuffer(EnumAttachment eAttachment, EnumInternalFormat eFormat)
    {
        unsigned int iRenderbuffer = 0;
        glGenRenderbuffers(1, &iRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, iRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, CGLHelper::toGLType(eFormat), m_iWidth, m_iHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, m_iID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, CGLHelper::toGLType(eAttachment), GL_RENDERBUFFER, iRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
        m_RenderBuffers[eAttachment] = iRenderbuffer;
    }

#ifdef DESKTOP_TARGET // Not supported on OpenGL ES
    void setDrawBuffers(const QList<EnumDrawBuffer>& drawBuffers)
    {
        if (drawBuffers.size() > 0)
        {
            QVector<GLenum> buffers;

            foreach (EnumDrawBuffer eDrawBuffer, drawBuffers)
            {
                buffers << CGLHelper::toGLType(eDrawBuffer);
            }

            glDrawBuffers(drawBuffers.size(), buffers.constData());
        }
    }
#endif // DESKTOP_TARGET

    int getWidth() const { return m_iWidth; }

    int getHeight() const { return m_iHeight; }

    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_iID);
    }

    void bindRead()
    {
#ifdef DESKTOP_TARGET
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_iID);
#endif
    }

    void bindDraw()
    {
#ifdef DESKTOP_TARGET
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_iID);
#endif
    }

    void release()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool isValid()
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    bool checkFrameBufferStatus()
    {
#ifdef DESKTOP_TARGET // Not supported on OpenGL ES
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch(status)
        {
        case GL_NO_ERROR:
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            qDebug("Unsupported framebuffer format.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            qDebug("Framebuffer incomplete attachment.");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            qDebug("Framebuffer incomplete, missing attachment.");
            break;
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
        case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
            qDebug("Framebuffer incomplete, duplicate attachment.");
            break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            qDebug("Framebuffer incomplete, attached images must have same dimensions.");
            break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_FORMATS
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
            qDebug("Framebuffer incomplete, attached images must have same format.");
            break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            qDebug("Framebuffer incomplete, missing draw buffer.");
            break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            qDebug("Framebuffer incomplete, missing read buffer.");
            break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            qDebug("Framebuffer incomplete, attachments must have same number of samples per pixel.");
            break;
#endif
        default:
            qDebug() <<"CGLFrameBuffer: An undefined error has occurred: "<< status;
            break;
        }
#endif // DESKTOP_TARGET
        return false;
    }

    QImage toImage()
    {
        QImage img(QSize(m_iWidth, m_iHeight), QImage::Format_RGB32);


        while (glGetError());

#ifdef EMBEDDED_TARGET
        glReadPixels(0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
#else
        glReadPixels(0, 0, m_iWidth, m_iHeight, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());
        if (glGetError())
        {
            glReadPixels(0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
            img = img.rgbSwapped();
        }
#endif
        return img.mirrored();
    }

private:

    unsigned int m_iID;
    unsigned int m_iWidth;
    unsigned int m_iHeight;

    QMap<EnumAttachment, int> m_RenderBuffers;
};

#endif // CGLFRAMEBUFFER_H

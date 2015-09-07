#ifndef CGLGLOBAL_H
#define CGLGLOBAL_H

#ifdef EMBEDDED_TARGET

    // OpenGL ES 2 is used
    #include <QOpenGLFunctions>
    #define CGLFunctions QOpenGLFunctions
    // Some OpenGL ES 2.0 device can't have more than 65536 vertices indexed
    #define GL_TYPE_SIZE GL_UNSIGNED_SHORT

#else // DESKTOP_TARGET

    // OpenGL 3.3 is used
    #include <QOpenGLFunctions_3_3_Core>
    #define CGLFunctions QOpenGLFunctions_3_3_Core
    #define DESKTOP_TARGET
    #define GL_TYPE_SIZE GL_UNSIGNED_INT

#endif // EMBEDDED_TARGET

#include <QOpenGLContext>

#ifdef MULTITHREAD_RENDERING
    #include "CGLContext.h"
#endif // MULTITHREAD_RENDERING

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLVertexArrayObject>

#define CGLWidget                   QOpenGLWidget
#define CGLFormat                   QSurfaceFormat
#define CGLShaderProgram			QOpenGLShaderProgram
#define CGLShader					QOpenGLShader
#define CGLBuffer					QOpenGLBuffer
#define CGLVertexArrayObject		QOpenGLVertexArrayObject
#define CGLFramebufferObject		QOpenGLFramebufferObject
#define CGLFramebufferObjectFormat	QOpenGLFramebufferObjectFormat


#endif // CGLGLOBAL_H

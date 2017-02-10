#ifndef CGLGLOBAL_H
#define CGLGLOBAL_H

#ifdef EMBEDDED_TARGET
    #include <QOpenGLFunctions>
    #define CGLFunctions QOpenGLFunctions
#else // DESKTOP_TARGET
    #define DESKTOP_TARGET
    #include <QOpenGLFunctions_3_3_Core>
    #define CGLFunctions QOpenGLFunctions_3_3_Core
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

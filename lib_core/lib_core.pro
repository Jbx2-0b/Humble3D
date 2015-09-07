include(../Global.pri)

TARGET = lib_core
TEMPLATE = lib

QT += concurrent

DEFINES += LIB_CORE_LIBRARY


SOURCES += \
    AEntity.cpp \
    CAnimation.cpp \
    CAnimationManager.cpp \
    CCamera.cpp \
    CFrustum.cpp \
    CGeometryGlobal.cpp \
    CIDGenerator.cpp \
    CLight.cpp \
    CMaterial.cpp \
    CMaterialManager.cpp \
    CMeshBuffer.cpp \
    CNameGenerator.cpp \
    CSceneManager.cpp \
    CSceneNode.cpp \
    CSceneNodeAnimation.cpp \
    CShader.cpp \
    CSubMesh.cpp \
    CRenderPass.cpp \
    CRenderBufferManager.cpp \
    CMesh.cpp \
    CTextureManager.cpp \
    CShaderManager.cpp \
    CMeshInstance.cpp \
    CSubMeshInstance.cpp \
    ASceneNodeItem.cpp \
    CMeshManager.cpp \
    ARenderableItem.cpp \
    COctreeNode.cpp \
    CGeometryInstancer.cpp \
    COctree.cpp \
    CBillboard.cpp \
    CGeometryTool.cpp \
    Shapes/CCoordinateSystem.cpp \
    Shapes/CLineBox.cpp \
    Shapes/CPlaneMesh.cpp \
    Shapes/CPolyLine.cpp \
    Shapes/CSolidBox.cpp \
    Shapes/CSphereMesh.cpp \
    CDebugManager.cpp \
    Shapes/CCylinderMesh.cpp

HEADERS += \
    AEntity.h \
    ARenderableItem.h \
    ARenderer.h \
    ATexture.h \
    CAnimation.h \
    CAnimationHelper.h \
    CAnimationManager.h \
    CBinaryMesh.h \
    CBlending.h \
    CBone.h \
    CBuffer.h \
    CCamera.h \
    CColorMask.h \
    CDepthRange.h \
    CDepthTest.h \
    CFaceCulling.h \
    CFrustum.h \
    CGeometryGlobal.h \
    CIDGenerator.h \
    CLight.h \
    CMaterial.h \
    CMaterialManager.h \
    CMeshAnimation.h \
    CMeshBuffer.h \
    CMeshKey.h \
    CNameGenerator.h \
    CQuaternionKey.h \
    CRenderQueue.h \
    CRenderStates.h \
    CSceneManager.h \
    CSceneNode.h \
    CSceneNodeAnimation.h \
    CScissorTest.h \
    CShader.h \
    CStencilState.h \
    CStencilTestFace.h \
    CSubMesh.h \
    CTexture2D.h \
    CTexture3D.h \
    CTextureCube.h \
    CVectorKey.h \
    CVertexWeight.h \
    lib_core_global.h \
    CRenderBuffer.h \
    CTextureTarget.h \
    CRenderPass.h \
    CRenderBufferManager.h \
    CFrameBuffer.h \
    CMesh.h \
    CTextureManager.h \
    CShaderManager.h \
    ASceneNodeItem.h \
    CMeshInstance.h \
    CSubMeshInstance.h \
    CMeshManager.h \
    CNode.h \
    COctreeNode.h \
    CGeometryInstancer.h \
    COctree.h \
    CBillboard.h \
    CGeometryTool.h \
    Shapes/CCoordinateSystem.h \
    Shapes/CLineBox.h \
    Shapes/CPlaneMesh.h \
    Shapes/CPolyLine.h \
    Shapes/CSolidBox.h \
    Shapes/CSphereMesh.h \
    CDebugManager.h \
    Shapes/CCylinderMesh.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/release/ -llib_tools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/debug/ -llib_tools
else:unix: LIBS += -L$$OUT_PWD/../lib_tools/ -llib_tools

INCLUDEPATH += $$PWD/../lib_tools
DEPENDPATH += $$PWD/../lib_tools

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_math/release/ -llib_math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_math/debug/ -llib_math
else:unix: LIBS += -L$$OUT_PWD/../lib_math/ -llib_math

INCLUDEPATH += $$PWD/../lib_math
DEPENDPATH += $$PWD/../lib_math

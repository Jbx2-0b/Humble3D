// App
#include "CGLRenderer.h"
#include "CLogManager.h"
#include "CGLTexture2D.h"
#include "CGLTextureTarget.h"
#include "CGLTextureCube.h"
#include "CGLFrameBuffer.h"

// Qt
#include <QVector2D>
#include <QVector3D>

//-----------------------------------------------------------------------------------------
CGLRenderer::CGLRenderer(CSceneManager* pSceneManager)
    : ARenderer(pSceneManager)
    , m_pCurrentShader(0)
    , m_iDrawCalls(0)
    , m_iPolygonsPerFrame(0)
{
    m_Time.start();

    setName("OpenGL Renderer");

#ifdef EMBEDDED_TARGET
    pLog->addMessage(eINFO, "Creating OpenGL Renderer - Embedded Version");
#else
    pLog->addMessage(eINFO, "Creating OpenGL Renderer - Desktop Version");
#endif

    pSceneManager->registerListener(this);
    CMeshManager::getInstance()->registerMeshBufferListener(this);
    CShaderManager::getInstance()->registerListener(this);
    CTextureManager::getInstance()->registerListener(this);
}

//-----------------------------------------------------------------------------------------
CGLRenderer::~CGLRenderer()
{
#ifdef MULTITHREAD_RENDERING
    m_GeometryInstancer.stop();
#endif // MULTITHREAD_RENDERING

    m_pSceneManager->unregisterListener(this);
    CMeshManager::getInstance()->unregisterMeshBufferListener(this);
    CShaderManager::getInstance()->unregisterListener(this);
    CTextureManager::getInstance()->unregisterListener(this);

    // On supprime le stuff OpenGL
    foreach (AGLTexture* pGLTexture, m_Textures)
    {
        delete pGLTexture;
    }

    m_Textures.clear();

    foreach (CGLFrameBuffer* pGLFrameBuffer, m_FrameBuffers)
    {
        delete pGLFrameBuffer;
    }

    m_FrameBuffers.clear();

    foreach (CGLShaderProgram* pGLShaderProgram, m_ShaderPrograms)
    {
        delete pGLShaderProgram;
    }

    m_ShaderPrograms.clear();

    foreach (CGLMeshBuffer* pGLMeshBuffer, m_HardwareBuffers.values())
    {
        delete pGLMeshBuffer;
    }

    m_HardwareBuffers.clear();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::registerListener(IGLRendererListener* pListener)
{
    m_GLRendererListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::unregisterListener(IGLRendererListener* pListener)
{
    m_GLRendererListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
// Implementation ARenderer
//-----------------------------------------------------------------------------------------
bool CGLRenderer::init()
{
    initializeOpenGLFunctions();

#ifdef MULTITHREAD_RENDERING
    m_PrincipalContext.init();
    m_GeometryInstancer.init(this, &m_PrincipalContext);
#endif // MULTITHREAD_RENDERING

    forceGLStates(m_CurrentRenderStates);
    checkError("init()");

    return ARenderer::init();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::render()
{
    if (!m_pSceneManager)
        return;

    if (isEnabled() && isDirty())
    {
        m_iDrawCalls = 0;
        m_iPolygonsPerFrame = 0;
        m_iMaterialBind = 0;
        m_iShaderBind = 0;

        // Need to restore Depth write before clear it
        glDepthMask(GL_TRUE);

        // Set background color
        glClearColor(m_BackgroundColor.x(), m_BackgroundColor.y(), m_BackgroundColor.z(), m_BackgroundColor.w());

        // Efface le buffer GL
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ////////////////////////////////////////////////////////////////////////////
        // Rendu des ARenderableItem
        ////////////////////////////////////////////////////////////////////////////

        // Items opaques
        renderItems(m_pSceneManager->getRenderQueue());

        // Items transparents
        renderItems(m_pSceneManager->getTransparentRenderQueue());

        wash();
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::renderMeshBuffer(CMeshBuffer* pBuffer)
{
    if (!m_pSceneManager)
        return;

    if (!ARenderer::isInit())
        return;

    if (!m_HardwareBuffers.contains(pBuffer))
    {
        createVertexBufferObject(pBuffer);
    }
    else if (pBuffer->isDirty())
    {
        updateVertexBufferObject(pBuffer);
    }

    renderVertexBufferObject(pBuffer);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::renderMeshBuffer(const QList<CMeshBuffer*>& buffers)
{
    if (!m_pSceneManager)
        return;

    if (!ARenderer::isInit())
        return;

    foreach (CMeshBuffer* pBuffer, buffers)
    {
        if (!pBuffer->isLocked())
        {
            if (!m_HardwareBuffers.contains(pBuffer))
            {
                createVertexBufferObject(pBuffer);
            }
            else if (pBuffer->isDirty())
            {
                updateVertexBufferObject(pBuffer);
            }

            renderVertexBufferObject(pBuffer);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateRenderStates()
{
    // Culling
    CFaceCulling& currentFaceCulling = m_CurrentRenderStates.faceCulling();
    const CFaceCulling& requestFaceCulling = m_RequestRenderStates.getFaceCulling();

    if (currentFaceCulling.isEnabled() != requestFaceCulling.isEnabled())
    {
        currentFaceCulling.setEnabled(requestFaceCulling.isEnabled());

        if (requestFaceCulling.isEnabled())
        {
            glEnable(GL_CULL_FACE);

            if (currentFaceCulling.getCullFace() != requestFaceCulling.getCullFace())
            {
                glCullFace(CGLHelper::toGLType(requestFaceCulling.getCullFace()));
                currentFaceCulling.setCullFace(requestFaceCulling.getCullFace());
            }
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }

    // ScissorTest
    CScissorTest& currentScissorTest = m_CurrentRenderStates.scissorTest();
    const CScissorTest& requestScissorTest = m_RequestRenderStates.getScissorTest();

    if (currentScissorTest.isEnabled() != requestScissorTest.isEnabled())
    {
        currentScissorTest.setEnabled(requestScissorTest.isEnabled());

        if (requestFaceCulling.isEnabled())
        {
            glEnable(GL_SCISSOR_TEST);

            currentScissorTest.setEnabled(requestScissorTest.isEnabled());

            if (currentScissorTest.getRect() != requestScissorTest.getRect())
            {
                QRect scissorRect = requestScissorTest.getRect();
                glScissor(scissorRect.x(), scissorRect.y(), scissorRect.width(), scissorRect.height());

                currentScissorTest.setRect(scissorRect);
            }
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }

    // DepthTest
    CDepthTest& currentDepthTest = m_CurrentRenderStates.depthTest();
    const CDepthTest& requestDepthTest = m_RequestRenderStates.getDepthTest();

    if (currentDepthTest.isEnabled() != requestDepthTest.isEnabled())
    {
        currentDepthTest.setEnabled(requestDepthTest.isEnabled());

        if (currentDepthTest.isEnabled())
        {
            glEnable(GL_DEPTH_TEST);

            if (currentDepthTest.getFunction() != requestDepthTest.getFunction())
            {
                glDepthFunc(CGLHelper::toGLType(requestDepthTest.getFunction()));
                currentDepthTest.setFunction(requestDepthTest.getFunction());
            }
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    // DepthRange
    CDepthRange& currentDepthRange = m_CurrentRenderStates.depthRange();
    const CDepthRange& requestDepthRange = m_RequestRenderStates.getDepthRange();

    if (currentDepthRange.getNear() != requestDepthRange.getNear() ||
            currentDepthRange.getFar() != requestDepthRange.getFar())
    {
#ifdef DESKTOP_TARGET
        glDepthRange(requestDepthRange.getNear(), requestDepthRange.getFar());
#endif // DESKTOP_TARGET
        currentDepthRange.setNear(requestDepthRange.getNear());
        currentDepthRange.setFar(requestDepthRange.getFar());
    }

    // Blending
    CBlending& currentBlending = m_CurrentRenderStates.blending();
    const CBlending& requestBlending = m_RequestRenderStates.getBlending();

    if (currentBlending.isEnabled() != requestBlending.isEnabled())
    {
        currentBlending.setEnabled(requestBlending.isEnabled());

        if (requestBlending.isEnabled())
        {
            glEnable(GL_BLEND);

            Source::EnumBlendingFactor eRequestSrcRGBFactor = requestBlending.getSourceRGBFactor();
            Destination::EnumBlendingFactor eRequestDstRGBFactor = requestBlending.getDestinationRGBFactor();
            Source::EnumBlendingFactor eRequestSrcAlphaFactor = requestBlending.getSourceAlphaFactor();
            Destination::EnumBlendingFactor eRequestDstAlphaFactor = requestBlending.getDestinationAlphaFactor();

            glBlendFuncSeparate(
                        CGLHelper::toGLType(eRequestSrcRGBFactor),
                        CGLHelper::toGLType(eRequestDstRGBFactor),
                        CGLHelper::toGLType(eRequestSrcAlphaFactor),
                        CGLHelper::toGLType(eRequestDstAlphaFactor));

            currentBlending.setSourceRGBFactor(eRequestSrcRGBFactor);
            currentBlending.setDestinationRGBFactor(eRequestDstRGBFactor);
            currentBlending.setSourceAlphaFactor(eRequestSrcAlphaFactor);
            currentBlending.setDestinationAlphaFactor(eRequestDstAlphaFactor);

            EnumBlendEquation eRequestRGBEquation = requestBlending.getRGBEquation();
            EnumBlendEquation eRequestAlphaEquation = requestBlending.getAlphaEquation();

            glBlendEquationSeparate(
                        CGLHelper::toGLType(eRequestRGBEquation),
                        CGLHelper::toGLType(eRequestAlphaEquation));

            currentBlending.setRGBEquation(eRequestRGBEquation);
            currentBlending.setAlphaEquation(eRequestAlphaEquation);


            const QVector4D& requestColor = requestBlending.getColor();

            glBlendColor(requestColor.x(), requestColor.y(), requestColor.z(), requestColor.w());
            currentBlending.setColor(requestColor);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    // ColorMask
    CColorMask& currentColorMask = m_CurrentRenderStates.colorMask();
    const CColorMask& requestColorMask = m_RequestRenderStates.getColorMask();

    if (currentColorMask != requestColorMask)
    {
        glColorMask(requestColorMask.isRedEnabled(), requestColorMask.isGreenEnabled(), requestColorMask.isBlueEnabled(), requestColorMask.isAlphaEnabled());
        currentColorMask.setRedEnabled(requestColorMask.isRedEnabled());
        currentColorMask.setGreenEnabled(requestColorMask.isGreenEnabled());
        currentColorMask.setBlueEnabled(requestColorMask.isBlueEnabled());
        currentColorMask.setAlphaEnabled(requestColorMask.isAlphaEnabled());
    }

    // DepthMask
    if (m_CurrentRenderStates.hasDepthMask() != m_RequestRenderStates.hasDepthMask())
    {
        if (m_RequestRenderStates.hasDepthMask())
        {
            glDepthMask(GL_TRUE);
            checkError("glDepthMask");
        }
        else
        {
            glDepthMask(GL_FALSE);
            checkError("glDepthMask");
        }

        m_CurrentRenderStates.setDepthMask(m_RequestRenderStates.hasDepthMask());
    }

    // StencilState
    CStencilState& currentStencilState = m_CurrentRenderStates.stencilState();
    const CStencilState& requestStencilState = m_RequestRenderStates.getStencilState();

    if (currentStencilState.isEnabled() != requestStencilState.isEnabled())
    {
        currentStencilState.setEnabled(requestStencilState.isEnabled());

        if (requestStencilState.isEnabled())
        {
            glEnable(GL_STENCIL_TEST);

            CStencilTestFace& currentStencilTestBackFace = currentStencilState.backFace();
            const CStencilTestFace& requestStencilTestBackFace = requestStencilState.getBackFace();

            glStencilFunc(CGLHelper::toGLType(requestStencilState.getStencilTestFunction()), requestStencilState.getReferenceValue(), requestStencilState.getMask());

            if (currentStencilTestBackFace != requestStencilTestBackFace)
            {
                glStencilOpSeparate(
                            GL_BACK,
                            CGLHelper::toGLType(requestStencilTestBackFace.getStencilFailOperation()),
                            CGLHelper::toGLType(requestStencilTestBackFace.getDepthFailStencilPassOperation()),
                            CGLHelper::toGLType(requestStencilTestBackFace.getDepthPassStencilPassOperation()));

                currentStencilTestBackFace = requestStencilTestBackFace;
            }

            CStencilTestFace& currentStencilTestFrontFace = currentStencilState.frontFace();
            const CStencilTestFace& requestStencilTestFrontFace = requestStencilState.getFrontFace();

            if (currentStencilTestFrontFace != requestStencilTestFrontFace)
            {
                glStencilOpSeparate(
                            GL_FRONT,
                            CGLHelper::toGLType(requestStencilTestFrontFace.getStencilFailOperation()),
                            CGLHelper::toGLType(requestStencilTestFrontFace.getDepthFailStencilPassOperation()),
                            CGLHelper::toGLType(requestStencilTestFrontFace.getDepthPassStencilPassOperation()));

                currentStencilTestFrontFace = requestStencilTestFrontFace;
            }
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
    }
}

//-----------------------------------------------------------------------------------------
QString CGLRenderer::getGPUName()
{
    return QString((char*)glGetString(GL_RENDERER));
}

//-----------------------------------------------------------------------------------------
QString CGLRenderer::getGLSLVersion()
{
    return QString((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//-----------------------------------------------------------------------------------------
QString CGLRenderer::getOpenGLVersion()
{
    return QString((char*)glGetString(GL_VERSION));
}

//-------------------------------------------------------------------------------------------------
bool CGLRenderer::isExtensionSupported(const QString& strExtension)
{
    QString extensionString(reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));

    return extensionString.contains(strExtension, Qt::CaseInsensitive);
}

//-----------------------------------------------------------------------------------------
// Implementation CMeshManager::IMeshBufferListener
//-----------------------------------------------------------------------------------------
void CGLRenderer::onDeleteMeshBuffer(CMeshBuffer* pBuffer)
{
    if (m_HardwareBuffers.contains(pBuffer))
    {
        CGLMeshBuffer* pGPUBuffer = m_HardwareBuffers[pBuffer];
        m_HardwareBuffers.remove(pBuffer);
        pLog->addMessage(eDEBUGMEMORY, QString("Removing GPU Buffer (Count: %1)").arg(m_HardwareBuffers.count()));
        delete pGPUBuffer;

        // La destruction du buffer induit la liberation de la memoire occupee sur le serveur OpenGL
    }
}

//-----------------------------------------------------------------------------------------
// Implementation CTextureManager::ITextureManagerListener
//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateTexture(ATexture* pTexture)
{
    if (pTexture->isValid())
    {
        createTexture(pTexture);
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onDeleteTexture(ATexture* pTexture)
{
    QString textureName = pTexture->getName();

    if (m_Textures.contains(textureName))
    {
        m_Textures.remove(textureName);
    }
}

//-----------------------------------------------------------------------------------------
// Implementation CShaderManager::IShaderManagerListener
//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateShader(CShader* pShader)
{
    if (!ARenderer::isInit())
        return;

    if (m_ShaderPrograms.contains(pShader->getName()))
    {
        // Shader already exist ? we remove it
        delete m_ShaderPrograms[pShader->getName()];
    }

    bindShader(pShader);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onDeleteShader(CShader* pShader)
{
    if (!ARenderer::isInit())
        return;

    if (m_ShaderPrograms.contains(pShader->getName()))
    {
        pLog->addMessage(eINFO, "Removing shader: " + pShader->getName());

        CGLShaderProgram* pProgram = m_ShaderPrograms[pShader->getName()];

        m_ShaderPrograms.remove(pShader->getName());

        delete pProgram;
    }
}

//-----------------------------------------------------------------------------------------
// Implementation CSceneManagerManager::ISceneManagerListener
//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateCamera(CCamera* /*pCamera*/)
{
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateAnimation(CAnimation* /*pAnimation*/)
{
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onCreateSceneNode(CSceneNode* /*pSceneNode*/)
{
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onUpdateSceneNode(CSceneNode* /*pSceneNode*/)
{
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::onDeleteSceneNode(CSceneNode* /*pSceneNode*/)
{
    setDirty();
}

//--------------------------------------------------------------------------------------------
// Methodes de rendu
//--------------------------------------------------------------------------------------------
void CGLRenderer::renderItems(const CRenderQueue& renderQueue)
{
    QMap<const CCamera*, QSet<CSceneNode*> > cullingResultCache;

    const QList<CMaterial*> materials = CMaterialManager::getInstance()->getMaterials();

    foreach (CMaterial* pMaterial, materials)
    {
        foreach (CRenderPass* pPass, pMaterial->renderingPassList())
        {
            const CCamera* pCamera = pPass->getCamera() ? pPass->getCamera() : m_pCurrentCamera;

            if (pCamera)
            {
                QList<ARenderableItem*> items = renderQueue.getItemsByMaterialID(pMaterial->getID());

                if (!items.isEmpty() && isOneRenderable(items))
                {
                    if (CGLShaderProgram* pShaderProgram = getShaderProgram(pPass))
                    {
                        m_pCurrentShader = pShaderProgram;
                        m_pCurrentShader->bind();

                        CGLFrameBuffer* pFrameBuffer = 0;

                        // Rendu dans une texture
                        if (pPass->getTargetType() == eTargetTexture)
                        {
                            pFrameBuffer = getFrameBuffer(pPass->getFrameBuffer());

                            if (pFrameBuffer->isValid())
                            {
#ifdef DESKTOP_TARGET // Not supported before OpenGL ES 3.0
                                foreach (const CRenderOperation& op, pPass->getPreOperations())
                                {
                                    CGLFrameBuffer* pSrcFrameBuffer = getFrameBuffer(op.getSource());
                                    CGLFrameBuffer* pDstFrameBuffer = getFrameBuffer(op.getDestination());

                                    pSrcFrameBuffer ? pSrcFrameBuffer->bindRead() : glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                                    pDstFrameBuffer ? pDstFrameBuffer->bindDraw() : glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

                                    GLbitfield mask = 0;
                                    if (op.maskColorBuffer()) mask |= GL_COLOR_BUFFER_BIT;
                                    if (op.maskDepthBuffer()) mask |= GL_DEPTH_BUFFER_BIT;
                                    if (op.maskStencilBuffer()) mask |= GL_STENCIL_BUFFER_BIT;

                                    glBlitFramebuffer(
                                                0, 0,
                                                pDstFrameBuffer->getWidth() - 1, pDstFrameBuffer->getHeight() - 1,
                                                0, 0,
                                                pDstFrameBuffer->getWidth() - 1, pDstFrameBuffer->getHeight() - 1,
                                                mask,
                                                CGLHelper::toGLType(op.getFilter()));
                                }
#endif // DESKTOP_TARGET
                                glViewport(0, 0, pFrameBuffer->getWidth(), pFrameBuffer->getHeight());
                                pFrameBuffer->bind();
                            }
                            else
                            {
                                pLog->addMessage(eERROR, "GLRenderer::renderItems() Error : Invalid FrameBuffer !");
                                continue;
                            }
                        }
                        // Rendu direct a l'ecran
                        else
                        {
                            glViewport(0, 0, getWidth(), getHeight());
                        }

                        setRenderStates(pPass->getRenderStates());

                        if (pPass->clearBufferFlags() > 0)
                        {
                            GLbitfield mask = 0;
                            if (pPass->clearColorBuffer()) mask |= GL_COLOR_BUFFER_BIT;
                            if (pPass->clearDepthBuffer()) mask |= GL_DEPTH_BUFFER_BIT;
                            if (pPass->clearStencilBuffer()) mask |= GL_STENCIL_BUFFER_BIT;

                            const QVector4D& clearColor = pPass->getClearColor();
                            glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w());
                            glClear(mask);
                        }

                        QSet<CSceneNode*> visibleSceneNodes;

                        if (m_bFrustumCullingEnabled)
                        {
                            if (cullingResultCache.contains(pCamera))
                            {
                                visibleSceneNodes = cullingResultCache.value(pCamera);
                            }
                            else
                            {
                                visibleSceneNodes = m_pSceneManager->getVisibleSceneNodes(pCamera->getFrustum());
                                cullingResultCache.insert(pCamera, visibleSceneNodes);
                            }
                        }

                        bindCamera(pCamera);
                        bindTime();
                        bindLights(pCamera);
                        bindMaterial(pMaterial);
                        bindUserUniformValues(pPass);

                        foreach (ARenderableItem* pItem, items)
                        {
                            QSet<CSceneNode*> itemNodes = pItem->getNodes();
                            QSet<CSceneNode*> nodesToRender = m_bFrustumCullingEnabled ? visibleSceneNodes & itemNodes : itemNodes;

                            if (!nodesToRender.isEmpty() && !pItem->isLocked())
                            {
                                pItem->lock();
                                if (pItem->isVisible() && pItem->isRenderable())
                                {
                                    bindSkeleton(pItem);
                                    foreach (CSceneNode* pNode, nodesToRender)
                                    {
                                        bindNode(pCamera, pNode);
                                        pItem->doRender(this);
                                    }
                                }
                                pItem->unlock();
                            }
                        }

                        if (pPass->getTargetType() == eTargetTexture)
                        {
                            if (pPass->isSaveBitmapEnabled())
                            {
                                pFrameBuffer->toImage().save(pPass->getSaveBitmapFilePathName());
                            }
                            pFrameBuffer->release();
                        }
                        releaseMaterial(pMaterial);
                        m_pCurrentShader->release();
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
// RENDU VERTEX BUFFER OBJECTS
//-----------------------------------------------------------------------------------------
void CGLRenderer::renderVertexBufferObject(CMeshBuffer* pBuffer)
{
    m_iDrawCalls++;

    // BONES
    m_pCurrentShader->setUniformValue("hasSkeleton", pBuffer->hasSkeleton());

    m_iPolygonsPerFrame += pBuffer->getPolygonCount();

    CGLMeshBuffer* pGPUBuffer = m_HardwareBuffers.value(pBuffer);

    pGPUBuffer->setPrimitiveType(CGeometryGlobal::fromRasterizationMode(pBuffer->getPrimitiveType(), m_eRasterizationMode));
    pGPUBuffer->setPrimitiveIndex(pBuffer->getPrimitiveIndex());
    pGPUBuffer->draw(m_pCurrentShader);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindTime()
{
    if (!m_pSceneManager)
        return;

    m_pCurrentShader->setUniformValue("iGlobalTime", (GLfloat) (m_Time.elapsed() / 1000.));
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindCamera(const CCamera* pCamera)
{
    if (!m_pSceneManager)
        return;

    // On recupere la camera courante
    if (pCamera)
    {
        QMatrix4x4 projectionMatrix = pCamera->getProjectionMatrix();
        m_pCurrentShader->setUniformValue("projectionMatrix", projectionMatrix);

        QVector3D eyePosition = pCamera->getEyePosition();
        m_pCurrentShader->setUniformValue("eyePosition", eyePosition);
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindNode(const CCamera* pCamera, CSceneNode* pNode)
{
    if (!m_pSceneManager)
        return;

    if (pCamera)
    {
        // For each node we compute is own Model-View-Projection matrix by multipling is transformation matrix (model)
        // by camera transformation matrix (View-Projection)
        // We do same stuff for other useful matrix (Model-View, Normal matrix...)
        QMatrix4x4 modelViewProjectionMatrix = pCamera->getTransformation() * pNode->getGlobalTransformation();
        m_pCurrentShader->setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);

        QMatrix4x4 modelViewMatrix = pCamera->getViewMatrix() * pNode->getGlobalTransformation();
        m_pCurrentShader->setUniformValue("modelViewMatrix", modelViewMatrix);

        QMatrix4x4 viewInverseMatrix = pCamera->getViewMatrix().inverted() * pNode->getGlobalTransformation();
        m_pCurrentShader->setUniformValue("viewInverseMatrix", viewInverseMatrix);

        // Normal matrix is defined by "the transposed of the inverted model view matrix"
        QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
        m_pCurrentShader->setUniformValue("normalMatrix", normalMatrix);
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindLights(const CCamera* pCamera)
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    // TRAITEMENTS DES LUMIERES
    ////////////////////////////////////////////////////////////////////////////////////////////

    m_pCurrentShader->setUniformValue("sceneAmbient", m_pSceneManager->getAmbientColor());
    m_pCurrentShader->setUniformValue("lightCount", m_pSceneManager->getLightCount());

    int iLightID = 0;

    foreach (CLight* pLight, m_pSceneManager->getLights())
    {
        if (CSceneNode* pLightNode = pLight->getNode())
        {
            // On calcul la position de la lumiere dans le repere de l'oeil
            QVector4D lightPosition = pCamera->getViewMatrix() * pLightNode->getGlobalTransformation() * QVector4D(0.0, 0.0, 0.0, 1.0);

            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eIsEnabled)).constData(),				pLight->isEnabled());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, ePosition)).constData(),				lightPosition);
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eDirection)).constData(),				pLight->getDirection());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eAmbientColor)).constData(),			pLight->getAmbientColor());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eDiffuseColor)).constData(),			pLight->getDiffuseColor());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eSpecularColor)).constData(),			pLight->getSpecularColor());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eConstantAttenuation)).constData(),		(GLfloat)pLight->getConstantAttenuation());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eLinearAttenuation)).constData(),		(GLfloat)pLight->getLinearAttenuation());
            m_pCurrentShader->setUniformValue(pGLHelper->getLightParamStr(TLightParam(iLightID, eQuadraticAttenuation)).constData(),	(GLfloat)pLight->getQuadraticAttenuation());
            QMatrix4x4 lightViewMatrix = pLight->getViewMatrix() * pLightNode->getGlobalTransformation();
            m_pCurrentShader->setUniformValue("lightViewMatrix", lightViewMatrix);

            iLightID++;
        }
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindMaterial(CMaterial* pMaterial)
{
    m_iMaterialBind++;

    m_pCurrentShader->setUniformValue("material.ambient",		pMaterial->getAmbientColor());
    m_pCurrentShader->setUniformValue("material.diffuse",		pMaterial->getDiffuseColor());
    m_pCurrentShader->setUniformValue("material.specular",		pMaterial->getSpecularColor());
    m_pCurrentShader->setUniformValue("material.alphaMask",		pMaterial->getAlphaMaskColor());
    m_pCurrentShader->setUniformValue("material.alphaMaskFunc",	(int)pMaterial->getAlphaMaskFunc());
    m_pCurrentShader->setUniformValue("material.shininess",		(GLfloat)pMaterial->getShininessFactor());
    m_pCurrentShader->setUniformValue("material.opacity",		(GLfloat)pMaterial->getOpacity());

    m_MaterialParameterCount[eDiffuse]		= 0;
    m_MaterialParameterCount[eSpecular]		= 0;
    m_MaterialParameterCount[eAmbient]		= 0;
    m_MaterialParameterCount[eEmissive]		= 0;
    m_MaterialParameterCount[eHeight]		= 0;
    m_MaterialParameterCount[eNormals]		= 0;
    m_MaterialParameterCount[eShininess]	= 0;
    m_MaterialParameterCount[eOpacity]		= 0;
    m_MaterialParameterCount[eDisplacement]	= 0;
    m_MaterialParameterCount[eLightmap]		= 0;
    m_MaterialParameterCount[eReflection]	= 0;

    foreach (const CTextureParam& texture, pMaterial->getTextureParams())
    {
        if (ATexture* pTexture = CTextureManager::getInstance()->getTextureByName(texture.getTextureName()))
        {
            EnumMaterialParameter eType = texture.getMaterialParameter();
            int iUnit = pTexture->getTextureUnit();

            switch (pTexture->getType())
            {
            case eTexture2D:
            case eTextureTarget:
            {
                if (AGLTexture* pGLTexture = getTexture(pTexture))
                {
                    if (pGLTexture->isValid())
                    {
                        glActiveTexture(GL_TEXTURE0 + iUnit);
                        m_pCurrentShader->setUniformValue(QString("%1%2").arg(pGLHelper->fromType(eType)).arg(m_MaterialParameterCount[eType]).toLatin1().constData(), iUnit);
                        pGLTexture->bind();
                        m_MaterialParameterCount[eType]++;
                    }
                }
            }
                break;

            case eTexture3D:
            {
                // TODO
            }
                break;

            case eTextureCube:
            {
                if (AGLTexture* pGLTexture = getTexture(pTexture))
                {
                    if (pGLTexture->isValid())
                    {
                        glActiveTexture(GL_TEXTURE0 + iUnit);
                        m_pCurrentShader->setUniformValue("cubeMapTex", iUnit);
                        pGLTexture->bind();
                    }
                }
            }
                break;

            case eTextureUndefined:
            default:
            {
            }
                break;
            }
        }
    }

    QMapIterator<EnumMaterialParameter, int> it(m_MaterialParameterCount);
    while (it.hasNext())
    {
        it.next();
        m_pCurrentShader->setUniformValue(pGLHelper->fromTypeCount(it.key()).toLatin1().constData(), it.value());
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindShader(CShader* pShader)
{
    if (pShader->isValid())
    {
        m_iShaderBind++;

        pLog->addMessage(eINFO, QString("GLRenderer: Linking shader %1...").arg(pShader->getName()));

        CGLShaderProgram* pNewShaderProgram = new CGLShaderProgram();

        m_ShaderPrograms[pShader->getName()] = pNewShaderProgram;

        // Override system locale until shaders are compiled
        setlocale(LC_NUMERIC, "C");

        QString log;

        if (pShader->hasVertexShader())
        {
            if (!pNewShaderProgram->addShaderFromSourceCode(CGLShader::Vertex, pShader->getVertexShaderCode()))
            {
                log += "Vertex shader: " + pNewShaderProgram->log();
            }
        }

        if (pShader->hasGeometryShader())
        {
            if (!pNewShaderProgram->addShaderFromSourceCode(CGLShader::Geometry,	pShader->getGeometryShaderCode()))
            {
                log += "Geometry shader: " + pNewShaderProgram->log();
            }
        }

        if (pShader->hasFragmentShader())
        {
            if (!pNewShaderProgram->addShaderFromSourceCode(CGLShader::Fragment,	pShader->getFragmentShaderCode()))
            {
                log += "Fragment shader: " + pNewShaderProgram->log();
            }
        }

        // Restore system locale
        setlocale(LC_ALL, "");

        static bool bIsOnError = false;

        if (!log.isEmpty())
        {
            bIsOnError = true;
        }
        else if (bIsOnError)
        {
            bIsOnError = false;
        }

        notifyNewMessage(bIsOnError, log);

        pNewShaderProgram->link();
    }

}

//-----------------------------------------------------------------------------------------
// INITIALISATION
//-----------------------------------------------------------------------------------------
void CGLRenderer::createVertexBufferObject(CMeshBuffer* pBuffer)
{
    if (!m_pSceneManager)
        return;

    if (!ARenderer::isInit())
        return;
    
    CGLMeshBuffer* m_pGLMeshBuffer = new CGLMeshBuffer();
    m_HardwareBuffers[pBuffer] = m_pGLMeshBuffer;

#ifdef MULTITHREAD_RENDERING

    m_PrincipalContext.doneCurrent();
    m_GeometryInstancer.addMeshBuffer(m_pGLMeshBuffer, pBuffer);
    pBuffer->wash();
    m_PrincipalContext.makeCurrent();

#else // SINGLE_THREADED

    m_pGLMeshBuffer->create(pBuffer);
    pBuffer->wash();

#endif // MULTITHREAD_RENDERING
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::updateVertexBufferObject(CMeshBuffer* pBuffer)
{
    if (!m_pSceneManager)
        return;

    if (!ARenderer::isInit())
        return;

    CGLMeshBuffer* pGPUBuffer = m_HardwareBuffers[pBuffer];
    pGPUBuffer->update(pBuffer);

    pBuffer->wash();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::createTexture(const ATexture* pTexture)
{
    if (!ARenderer::isInit())
        return;

    if (m_Textures.contains(pTexture->getName())) // texture already exist ?
    {
        pLog->addMessage(eDEBUG, "GLRenderer:  : " + pTexture->getName() + " removed for update.");
        AGLTexture* pGLTexture = m_Textures[pTexture->getName()];
        m_Textures.remove(pTexture->getName());
        delete pGLTexture;
    }

    AGLTexture* pGLTexture = 0;

    if (const CTexture2D* pTexture2D = dynamic_cast<const CTexture2D*>(pTexture))
    {
        bool bIsEmpty = pTexture2D->imageRects().isEmpty();

        QSize texSize = pTexture2D->getSize();

        if (bIsEmpty)
        {
            pGLTexture = new CGLTexture2D(texSize.width(), texSize.height());
        }
        else
        {
            CGLTexture2D* pGLTexture2D = new CGLTexture2D(texSize.width(), texSize.height());
            pGLTexture = pGLTexture2D;

            foreach (const SImageRect& imgRect, pTexture2D->imageRects())
            {
                pGLTexture2D->loadSub(imgRect.first, imgRect.second);
            }
        }
    }
    else if (const CTextureTarget* pTextureTarget = dynamic_cast<const CTextureTarget*>(pTexture))
    {
        QSize texSize = pTextureTarget->getSize();
        pGLTexture = new CGLTextureTarget(texSize.width(), texSize.height(), pTextureTarget->getFormat());
    }
    else if (const CTextureCube* pTextureCube = dynamic_cast<const CTextureCube*>(pTexture))
    {
        pGLTexture = new CGLTextureCube(pTextureCube->getFileNames());
    }

    if (pGLTexture)
    {
        m_Textures.insert(pTexture->getName(), pGLTexture);

        if (!pGLTexture->isValid())
        {
            pLog->addMessage(eWARN, "GLRenderer::addTexture() : Error loading : " + pTexture->getName());
        }
        else
        {
            pLog->addMessage(eDEBUG, "GLRenderer:  : " + pTexture->getName() + " loading succefully.");
            pLog->addMessage(eDEBUG, "GLRenderer: Add OpenGL texture, id [" + QString::number(pGLTexture->getID()) + "], ("+
                             QString::number(pGLTexture->getWidth()) + " * " + QString::number(pGLTexture->getHeight()) + ")");
        }
    }
    else
    {
        pLog->addMessage(eWARN, "GLRenderer: Texture type not supported" + pTexture->getName());
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::createFrameBuffer(CFrameBuffer* pFrameBuffer)
{
    if (!ARenderer::isInit())
        return;

    if (m_FrameBuffers.contains(pFrameBuffer)) // FrameBuffer already exist ?
    {
        pLog->addMessage(eDEBUG, "GLRenderer : FrameBuffer - suppression pour mise a jour.");
        CGLFrameBuffer* pGLFrameBuffer = m_FrameBuffers[pFrameBuffer];
        m_FrameBuffers.remove(pFrameBuffer);
        delete pGLFrameBuffer;
    }

    CGLFrameBuffer* pGLFrameBuffer = new CGLFrameBuffer(pFrameBuffer->getWidth(), pFrameBuffer->getHeight());

    int iTextureCount = 0;
    QHashIterator<EnumAttachment, CTextureTarget*> itTextureTarget(pFrameBuffer->getTextureTargets());
    while (itTextureTarget.hasNext())
    {
        itTextureTarget.next();

        EnumAttachment eAttachment = itTextureTarget.key();
        CTextureTarget* pTexture = itTextureTarget.value();
        createTexture(pTexture);
        pGLFrameBuffer->attachTexture(getTexture(pTexture)->getID(), eAttachment);

        iTextureCount++;
    }

    int iRenderBufferCount = 0;
    QHashIterator<EnumAttachment, CRenderBuffer*> itRenderBuffer(pFrameBuffer->getRenderBuffers());
    while (itRenderBuffer.hasNext())
    {
        itRenderBuffer.next();
        EnumAttachment eAttachment = itRenderBuffer.key();
        CRenderBuffer* pRenderBuffer = itRenderBuffer.value();

        pGLFrameBuffer->attachRenderbuffer(eAttachment, pRenderBuffer->getInternalFormat());

        iRenderBufferCount++;
    }

#ifdef DESKTOP_TARGET // Not supported before OpenGL ES 3.0
    pGLFrameBuffer->setDrawBuffers(pFrameBuffer->getDrawBuffers());
#endif // DESKTOP_TARGET

    pGLFrameBuffer->checkFrameBufferStatus();

    m_FrameBuffers.insert(pFrameBuffer, pGLFrameBuffer);

    if (pGLFrameBuffer->isValid())
    {
        pLog->addMessage(eDEBUG, "GLRenderer : FrameBuffer created succefully." );
        pLog->addMessage(eDEBUG, "GLRenderer : FrameBuffer textures : " + QString::number(iTextureCount));
        pLog->addMessage(eDEBUG, "GLRenderer : FrameBuffer renderbuffers : "  + QString::number(iRenderBufferCount));

    }
    else
    {
        pLog->addMessage(eDEBUG, "GLRenderer : Failure when creating.");
    }

    pGLFrameBuffer->release();
}

//-----------------------------------------------------------------------------------------
AGLTexture* CGLRenderer::getTexture(const ATexture* pTexture)
{
    if (pTexture->isValid())
    {
        QString textureName = pTexture->getName();

        if (!m_Textures.contains(textureName))
        {
            createTexture(pTexture);
        }

        return m_Textures.value(textureName);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CGLFrameBuffer* CGLRenderer::getFrameBuffer(CFrameBuffer* pFrameBuffer)
{
    if (!m_FrameBuffers.contains(pFrameBuffer))
    {
        createFrameBuffer(pFrameBuffer);
    }

    return m_FrameBuffers.value(pFrameBuffer);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::releaseMaterial(CMaterial* pMaterial)
{
    foreach (const CTextureParam& texture, pMaterial->getTextureParams())
    {
        if (ATexture* pTexture = CTextureManager::getInstance()->getTextureByName(texture.getTextureName()))
        {
            if (AGLTexture* pGLTexture = getTexture(pTexture))
            {
                if (pGLTexture->isValid())
                {
                    pGLTexture->release();
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindUserUniformValues(const CRenderPass* pPass)
{
    if (CShader* pShader = CShaderManager::getInstance()->getShaderByName(pPass->getShaderName()))
    {
        foreach (const TUniformValue& uniformValue, pShader->getUniformValues())
        {
            QByteArray name = uniformValue.first.toLatin1();

            switch (int(uniformValue.second.type()))
            {
            case QMetaType::Int:
                m_pCurrentShader->setUniformValue(name.constData(), uniformValue.second.toInt());
                break;
            case QMetaType::Float:
                m_pCurrentShader->setUniformValue(name.constData(), (GLfloat)uniformValue.second.toFloat());
                break;
            case QMetaType::Double:
                m_pCurrentShader->setUniformValue(name.constData(), (GLfloat)uniformValue.second.toDouble());
                break;
            case QMetaType::QVector2D:
                m_pCurrentShader->setUniformValue(name.constData(), uniformValue.second.value<QVector2D>());
                break;
            case QMetaType::QVector3D:
                m_pCurrentShader->setUniformValue(name.constData(), uniformValue.second.value<QVector3D>());
                break;
            case QMetaType::QVector4D:
                m_pCurrentShader->setUniformValue(name.constData(), uniformValue.second.value<QVector4D>());
                break;
            case QMetaType::QMatrix4x4:
                m_pCurrentShader->setUniformValue(name.constData(), uniformValue.second.value<QMatrix4x4>());
                break;
            default:
                break;
            }
        }

        foreach (const TUniformArray& uniformArray, pShader->getUniformValueArrays())
        {
            QVariant array = uniformArray.second;
            QByteArray name = uniformArray.first.toLatin1();

            if (array.canConvert<QVector<float> >())
            {
                const QVector<float>& list = array.value<QVector<float> >();
                m_pCurrentShader->setUniformValueArray(name.constData(), (const GLfloat*)list.constData(), list.size(), 1);
            }
            else if (array.canConvert<QVector<QVector2D> >())
            {
                const QVector<QVector2D>& list = array.value<QVector<QVector2D> >();
                m_pCurrentShader->setUniformValueArray(name.constData(), list.constData(), list.size());
            }
            else if (array.canConvert<QVector<QVector3D> >())
            {
                const QVector<QVector3D>& list = array.value<QVector<QVector3D> >();
                m_pCurrentShader->setUniformValueArray(name.constData(), list.constData(), list.size());
            }
            else if (array.canConvert<QVector<QVector4D> >())
            {
                const QVector<QVector4D>& list = array.value<QVector<QVector4D> >();
                m_pCurrentShader->setUniformValueArray(name.constData(), list.constData(), list.size());
            }
            else if (array.canConvert<QVector<QMatrix4x4> >())
            {
                const QVector<QMatrix4x4>& list = array.value<QVector<QMatrix4x4> >();
                m_pCurrentShader->setUniformValueArray(name.constData(), list.constData(), list.size());
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::bindSkeleton(ARenderableItem* pItem)
{
    if (pItem->hasSkeleton())
    {
        QVector<QMatrix4x4> bones = pItem->getBoneTransformations();
        m_pCurrentShader->setUniformValueArray("bones", bones.constData(), bones.count());
    }
}

//-----------------------------------------------------------------------------------------
CGLShaderProgram* CGLRenderer::getShaderProgram(const CRenderPass* pPass)
{
    QString shaderName = pPass->getShaderName();

    CGLShaderProgram* pShaderProgram = 0;

    if (CShader* pShader = CShaderManager::getInstance()->getShaderByName(shaderName))
    {
        if (!m_ShaderPrograms.contains(pShader->getName()))
        {
            bindShader(pShader);
        }

        pShaderProgram = m_ShaderPrograms.value(pShader->getName(), 0);
    }
    else
    {
        pLog->addMessage(eWARN, "Shader not found: " + shaderName);
    }

    return pShaderProgram;
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::forceGLStates(const CRenderStates& renderStates)
{	
    // Culling
    glCullFace(CGLHelper::toGLType(renderStates.getFaceCulling().getCullFace()));
    renderStates.getFaceCulling().isEnabled() ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

    checkError("glCullFace");

    // ScissorTest
    renderStates.getScissorTest().isEnabled() ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
    QRect scissorRect = renderStates.getScissorTest().getRect();
    glScissor(scissorRect.x(), scissorRect.y(), scissorRect.width(), scissorRect.height());

    checkError("glScissor");

    // Depth Func
    renderStates.getDepthTest().isEnabled() ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    glDepthFunc(CGLHelper::toGLType(renderStates.getDepthTest().getFunction()));

    checkError("glScissor");

    // Depth Range
#ifdef DESKTOP_TARGET
    glDepthRange(renderStates.getDepthRange().getNear(), renderStates.getDepthRange().getFar());
#endif //DESKTOP_TARGET

    checkError("glScissor");

    // Blending
    renderStates.getBlending().isEnabled() ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

    Source::EnumBlendingFactor eSrcRGBFactor = renderStates.getBlending().getSourceRGBFactor();
    Destination::EnumBlendingFactor eDstRGBFactor = renderStates.getBlending().getDestinationRGBFactor();
    Source::EnumBlendingFactor eSrcAlphaFactor = renderStates.getBlending().getSourceAlphaFactor();
    Destination::EnumBlendingFactor eDstAlphaFactor = renderStates.getBlending().getDestinationAlphaFactor();

    glBlendFuncSeparate(
                CGLHelper::toGLType(eSrcRGBFactor),
                CGLHelper::toGLType(eDstRGBFactor),
                CGLHelper::toGLType(eSrcAlphaFactor),
                CGLHelper::toGLType(eDstAlphaFactor));

    checkError("glBlendFuncSeparate");

    EnumBlendEquation eRGBEquation = renderStates.getBlending().getRGBEquation();
    EnumBlendEquation eAlphaEquation = renderStates.getBlending().getAlphaEquation();

    glBlendEquationSeparate(
                CGLHelper::toGLType(eRGBEquation),
                CGLHelper::toGLType(eAlphaEquation));

    checkError("glBlendEquationSeparate");

    const QVector4D& color = renderStates.getBlending().getColor();
    glBlendColor(color.x(), color.y(), color.z(), color.w());

    checkError("glBlendColor");

    // ColorMask
    const CColorMask& colorMask = renderStates.getColorMask();
    glColorMask(colorMask.isRedEnabled(), colorMask.isGreenEnabled(), colorMask.isBlueEnabled(), colorMask.isAlphaEnabled());

    checkError("glColorMask");

    // DepthMask
    glDepthMask(renderStates.hasDepthMask());

    checkError("glDepthMask");

    // StencilState
    renderStates.getStencilState().isEnabled() ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);

    checkError("glStencil");

    const CStencilTestFace& stencilTestBackFace = renderStates.getStencilState().getBackFace();
    const CStencilTestFace& stencilTestFrontFace = renderStates.getStencilState().getFrontFace();

    glStencilOpSeparate(
                GL_BACK,
                CGLHelper::toGLType(stencilTestBackFace.getStencilFailOperation()),
                CGLHelper::toGLType(stencilTestBackFace.getDepthFailStencilPassOperation()),
                CGLHelper::toGLType(stencilTestBackFace.getDepthPassStencilPassOperation()));

    checkError("glStencilOpSeparateBack");

    glStencilOpSeparate(
                GL_FRONT,
                CGLHelper::toGLType(stencilTestFrontFace.getStencilFailOperation()),
                CGLHelper::toGLType(stencilTestFrontFace.getDepthFailStencilPassOperation()),
                CGLHelper::toGLType(stencilTestFrontFace.getDepthPassStencilPassOperation()));

    checkError("glStencilOpSeparateFront");

    m_CurrentRenderStates = m_RequestRenderStates = renderStates;
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::checkError(const QString& message)
{
    GLenum glError = glGetError(); // Recupere les erreurs OpenGL

    switch (glError)
    {
    case GL_NO_ERROR:
        break;

    case GL_INVALID_ENUM:
        pLog->addMessage(eERROR, QString("%1 : Invalid enumerator").arg(message));
        break;

    case GL_INVALID_VALUE:
        pLog->addMessage(eERROR, QString("%1 : Invalid value").arg(message));
        break;

    case GL_INVALID_OPERATION:
        pLog->addMessage(eERROR, QString("%1 : Invalid operation").arg(message));
        break;

    case GL_OUT_OF_MEMORY:
        pLog->addMessage(eERROR, QString("%1 : Out of memory").arg(message));
        break;
#ifndef EMBEDDED_TARGET
    case GL_STACK_OVERFLOW:
        pLog->addMessage(eERROR, QString("%1 : Stack overflow").arg(message));
        break;

    case GL_STACK_UNDERFLOW:
        pLog->addMessage(eERROR, QString("%1 : Stack underflow").arg(message));
        break;

    case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
        pLog->addMessage(eERROR, QString("%1 : Invalid framebuffer operation").arg(message));
        break;
#endif
    default:
        pLog->addMessage(eERROR, QString("%1 : Unknow error").arg(message));
        break;
    }
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::notifyNewMessage(bool bIsError, const QString& message)
{
    foreach (IGLRendererListener* pListener, m_GLRendererListeners)
    {
        pListener->onNewMessage(bIsError, message);
    }
}

#ifdef MULTITHREAD_RENDERING
//-----------------------------------------------------------------------------------------
// GEOMETRY INSTANCER
//-----------------------------------------------------------------------------------------

CGLRenderer::CGLGeometryInstancer::CGLGeometryInstancer()
    : QThread()
    , m_pSharedContext(0)
    , m_pSurface(0)
    , m_pRenderer(0)
    , m_bRun(false)
    , m_Mutex(QMutex::Recursive)
{
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::CGLGeometryInstancer::init(CGLRenderer* pRenderer, CGLContext* pPrincipalContext)
{
    QMutexLocker locker(&m_Mutex);
    m_pRenderer = pRenderer;
    m_pSharedContext = pPrincipalContext->createSharedContext(this);
    m_bRun = true;
    start();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::CGLGeometryInstancer::stop()
{
    QMutexLocker locker(&m_Mutex);
    m_bRun = false;
}

//-----------------------------------------------------------------------------------------
CGLRenderer::CGLGeometryInstancer::~CGLGeometryInstancer()
{
    quit();
    wait();
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::CGLGeometryInstancer::addMeshBuffer(CGLMeshBuffer* pGLMeshBuffer, CMeshBuffer* pBuffer)
{
    QMutexLocker locker(&m_Mutex);
    pGLMeshBuffer->lock();
    m_MeshBufferList.append(QPair<CGLMeshBuffer*, CMeshBuffer*>(pGLMeshBuffer, pBuffer));
}

//-----------------------------------------------------------------------------------------
QPair<CGLMeshBuffer*, CMeshBuffer*> CGLRenderer::CGLGeometryInstancer::getAnAvailableMeshBuffer()
{
    if (!m_MeshBufferList.isEmpty())
    {
        int i = 0;
        while (true)
        {
            QPair<CGLMeshBuffer*, CMeshBuffer*> pMeshBuffer = m_MeshBufferList[i];
            if (!pMeshBuffer.second->isLocked())
            {
                QMutexLocker locker(&m_Mutex);
                m_MeshBufferList.removeAt(i);
                return pMeshBuffer;
            }
            else
            {
                msleep(s_iWaitTime);
            }

            if (++i == m_MeshBufferList.size()) i = 0;
        }
    }

    return QPair<CGLMeshBuffer*, CMeshBuffer*>(0, 0);
}

//-----------------------------------------------------------------------------------------
void CGLRenderer::CGLGeometryInstancer::run()
{
    while (m_bRun)
    {
        QPair<CGLMeshBuffer*, CMeshBuffer*> pMeshBuffer = getAnAvailableMeshBuffer();
        if (pMeshBuffer.first != 0 && pMeshBuffer.second != 0)
        {
            pMeshBuffer.first->lock();
            pMeshBuffer.second->lock();
            m_pSharedContext->makeCurrent();
            pMeshBuffer.first->create(pMeshBuffer.second);
            m_pSharedContext->doneCurrent();
            pMeshBuffer.first->unlock();
            pMeshBuffer.second->unlock();
            m_pRenderer->setDirty();
            pLog->addMessage(eDEBUGTHREAD, QString("CGLGeometryInstancer (Thread: %1): Push %2")
                             .arg((qlonglong)QThread::currentThreadId())
                             .arg(pMeshBuffer.second->getName()));
        }
        else
        {
            msleep(s_iWaitTime);
        }
    }
}

#endif // MULTITHREAD_RENDERING

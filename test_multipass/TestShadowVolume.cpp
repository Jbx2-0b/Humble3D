#include "TestShadowVolume.h"
#include "CMaterialManager.h"
#include "Shapes/CPlaneMesh.h"
#include "CRenderPass.h"
#include "CAssimpImporter.h"
#include "CGLRenderer.h"


static const QString resourcesDir = "G:/Humble3D/Deploy/scenes/";
static const int AppWidth = 600;
static const int AppHeight = 600;

//-----------------------------------------------------------------------------------------
TestShadowVolume::TestShadowVolume()
{
    CShaderManager::getInstance().createShader("blur", "://blur.vertex.glsl", "", "://blur.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "volume",
                "://shadowvolume-vol.vertex.glsl",
                "://shadowvolume-vol.geometry.glsl",
                "://shadowvolume-vol.fragment.glsl");

    CShader* pRenderShader = CShaderManager::getInstance().createShader(
                "render",
                "://shadowvolume-render.vertex.glsl",
                "",
                "://shadowvolume-render.fragment.glsl");

    CShaderManager::getInstance().createShader(
                "composite",
                "://shadowvolume-comp.vertex.glsl",
                "",
                "://shadowvolume-comp.fragment.glsl");

    // On charge le modéle
    CSceneNode* pRootNode = m_pSceneManager->getRootNode();

    CSceneNode* pModelNode = pRootNode->createChild("CowNode");

    if (!CAssimpImporter::mergeScene(resourcesDir + "cow.obj", m_pSceneManager, true, pModelNode).isEmpty())
    {
        m_pCenterNode = pRootNode->createChild("CenterNode");
        CSceneNode* pLightNode = m_pCenterNode->createChild("LightNode", QVector3D(5.0, 5.0, 0));

        // On crée une lumiére diffuse blanche
        CLight* pLight = m_pSceneManager->createLight("Light");
        pLight->setDiffuseColor(0.8f, 0.8f, 0.8f);
        pLight->setAmbientColor(0.6f, 0.6f, 0.6f);
        pLight->setSpecularColor(0.4f, 0.4f, 0.4f);
        pLight->setDirection(QVector3D(-1, -1, 0));

        // On l'associe au noeud
        pLightNode->addItem(pLight);

        CPlaneMesh* pWallMesh = CMeshManager::getInstance().createCustomMesh<CPlaneMesh>("CPlaneMesh", "WallMesh");
        pWallMesh->init(CPlaneMesh::ePlaneXY, 10., 10., 2, 2, 5., 5.);

        CMeshInstance* pWall1 = getSceneManager()->createMeshInstance(pWallMesh, "Wall1");
        CMeshInstance* pWall2 = getSceneManager()->createMeshInstance(pWallMesh, "Wall2");
        CMeshInstance* pWall3 = getSceneManager()->createMeshInstance(pWallMesh, "Wall3");

        CMaterial* pWallMat = CMaterialManager::getInstance().createMaterial("WallMat");
        ATexture* pWallTexture = CTextureManager::getInstance().createTexture2D("WallTex", resourcesDir + "brick.png");
        pWallMat->addTexture(pWallTexture, eDiffuse);

        pWall1->setMaterialName(pWallMat->getName());
        pWall2->setMaterialName(pWallMat->getName());
        pWall3->setMaterialName(pWallMat->getName());

        CSceneNode* pWall1Node = pRootNode->createChild("Wall1Node", QVector3D(0, 2.5, 5));
        pWall1Node->addItem(pWall1);

        CSceneNode* pWall2Node = pRootNode->createChild("Wall2Node", QVector3D(5, 2.5, 0), QQuaternion::fromAxisAndAngle(0, 1, 0, 90));
        pWall2Node->addItem(pWall2);

        CSceneNode* pWall3Node = pRootNode->createChild("Wall3Node", QVector3D(0, -2.5, 0), QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
        pWall3Node->addItem(pWall3);

        // On crée une camera
        CCamera* pCamera = getSceneManager()->createCamera();
        pCamera->setEyePosition(QVector3D(-6., 3., -6.));
        pCamera->setCenter(QVector3D(0., 0., 0.));
        pCamera->setAspectRatio((real)AppWidth / (real)AppHeight);

        CMaterial* pEffect = CMaterialManager::getInstance().createMaterial("Effect");

        // PASS 1
        CRenderPass* pFirstPass = pEffect->getRenderPass(0);

        pFirstPass->renderStates().setDepthMask(true);
        pFirstPass->renderStates().stencilState().setEnabled(false);
        pFirstPass->renderStates().depthTest().setEnabled(true);

        pFirstPass->setClearBufferFlags(ColorBuffer | DepthBuffer);

        CCamera* pFirstPassCamera = m_pSceneManager->createCamera("LightView");
        pFirstPassCamera->setInfinitePerspective(50., (real)AppWidth / (real)AppHeight, 0.5);
        pFirstPassCamera->setLookAt(QVector3D(5.0f, 5.0f, 5.0f), QVector3D(0, 2, 0), QVector3D(0, 1, 0));

        pFirstPass->setCamera(pFirstPassCamera);
        pFirstPass->setShaderName("render");

        real angle = 0.0f;
        QVector4D lightPos = QVector4D(5.0f * QVector3D(cosf(angle) * 7.5f, 1.5f, sinf(angle) * 7.5f), 1.0f);  // World coords
        pRenderShader->setUniformValue("LightPosition", pFirstPassCamera->getViewMatrix() * lightPos);

        pFirstPass->setTargetType(eTargetTexture);

        CFrameBuffer* pFirstPassFBO = pFirstPass->getFrameBuffer();
        pFirstPassFBO->createRenderBuffer(eAttachmentDepth, eInternalFormatDepthComponent);
        pFirstPassFBO->createRenderBuffer(eAttachmentColor0, eInternalFormatRGB);
        pFirstPassFBO->createRenderTexture(eAttachmentColor1, eInternalFormatRGB);
        pFirstPassFBO->setDrawBuffers(QList<EnumDrawBuffer>() << eDrawBufferAttachmentColor0 << eDrawBufferAttachmentColor1);

        CRenderPass* pSecondPass = pEffect->createRenderPass(eTargetScreen);
        pSecondPass->addPreOperation(CRenderOperation(pFirstPassFBO, 0, ColorBuffer | DepthBuffer, eLinear));
        pSecondPass->setShaderName("volume");
        pSecondPass->renderStates().setColorMask(CColorMask(false, false, false, false));
        pSecondPass->renderStates().setDepthMask(false);
        pSecondPass->setClearBufferFlags(StencilBuffer);
        pSecondPass->renderStates().stencilState().setEnabled(false);
        pSecondPass->renderStates().stencilState().setStencilTestFunction(eAlways, 0, 0xffff);
        pSecondPass->renderStates().stencilState().setBackFace(CStencilTestFace(eKeep, eKeep, eIncrementWrap));
        pSecondPass->renderStates().stencilState().setFrontFace(CStencilTestFace(eKeep, eKeep, eDecrementWrap));

        CRenderPass* pThirdPass = pEffect->createRenderPass(eTargetScreen);
        pThirdPass->setShaderName("composite");
        pThirdPass->renderStates().depthTest().setEnabled(false);
        pThirdPass->renderStates().blending().setEnabled(true);
        pThirdPass->renderStates().blending().setBlendFunction(Source::eOne, Destination::eOne);
        pSecondPass->renderStates().stencilState().setStencilTestFunction(eEqual, 0, 0xffff);
        pSecondPass->renderStates().stencilState().setBoth(CStencilTestFace(eKeep, eKeep, eKeep));

        CMeshInstance* pMeshInstance = getSceneManager()->findItemByName<CMeshInstance>("0_Instance");
        pMeshInstance->setMaterialName(pEffect->getName());

#ifdef POST_RENDERING

        CRenderPass* pPass = pMat->getRenderPass(0);

        pPass->setSize(QSize(512, 512));
        pPass->setTargetType(eTargetTexture);
        pPass->setClearColor(QVector4D(0.0, 0.0, 0.0, 1.0));
        pPass->setClearBufferFlags(ColorBuffer | DepthBuffer);

        CFrameBuffer* pFB = pPass->getFrameBuffer();
        ATexture* tex = pFB->createRenderTexture(eAttachmentColor0, eInternalFormatRGB);
        pFB->createRenderBuffer(eAttachmentDepth, eInternalFormatDepthComponent);

        pPass->setCamera(pCamera);

        CPlaneMesh* pFullScreenQuad = getSceneManager()->createCustomMesh<CPlaneMesh>("CPlaneMesh", "FullScreenQuad");
        pFullScreenQuad->init(CPlaneMesh::ePlaneXY, 1, 1, 1, 1);

        CMaterial* pFullScreenQuadMat = CMaterialManager::getInstance().createMaterial("FullScreenQuadMat");
        pFullScreenQuadMat->addTexture(tex, eDiffuse);
        pFullScreenQuadMat->getRenderPass(0)->setUserShadingModelName("blur");
        pFullScreenQuadMat->getRenderPass(0)->setShadingModel(eShadingModelUser);
        pFullScreenQuad->setMaterial(pFullScreenQuadMat);

        CSceneNode* pFullScreenQuadNode = pRootNode->createChild("FullScreenQuadNode");
        pFullScreenQuadNode ->addItem(pFullScreenQuad);

        // On crée une camera
        m_pSceneCamera = getSceneManager()->createCamera();
        m_pSceneCamera->setProjectionType(eProjectionOrtho);
        m_pSceneCamera->setOrtho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 2.f);

        m_pSceneCamera->setEyePosition(QVector3D(0., 0., -1.));
        m_pSceneCamera->setCenter(QVector3D(0., 0., 0.));

        CWidget3D* pView = createWidget3D(m_pSceneCamera);
        m_pSceneCamera->setAspectRatio((real) AppWidth / (real) AppHeight);

        QTimer* pTimer = new QTimer(this);
        connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
        pTimer->start(50);

        pView->setGeometry(QRect(100, 100, AppWidth, AppHeight));
        pView->getRenderer()->run();
        pView->move(0., 0.);
        //qDebug() << pPass->getCamera()->toString();
#endif


    }
}

//-----------------------------------------------------------------------------------------
TestShadowVolume::~TestShadowVolume()
{
}

//-----------------------------------------------------------------------------------------
void TestShadowVolume::onTimeout()
{
    static real angle = 2.;
    m_pCenterNode->rotate(QVector3D(0., 1., 0.), angle);
    getViews()[0]->getRenderer()->setDirty();
}

/*
 * #include "sceneshadowvolume.h"

#include <cstdio>

#include "glutils.h"
#include "defines.h"
#include "tgaio.h"

using glm::vec3;

#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneShadowVolume::SceneShadowVolume()
{
    width = 800;
    height = 600;
    rotSpeed = 0.5f;
    tPrev = 0.0f;
}

void SceneShadowVolume::initScene()
{
  compileAndLinkShader();

  glClearColor(0.5f,0.5f,0.5f,1.0f);
  glClearStencil(0);

  glEnable(GL_DEPTH_TEST);

  angle = 0.0f;

  plane = new VBOPlane(10.0f, 10.0f, 2, 2, 5.0f, 5.0f);
  spot = new VBOMeshAdj("../media/spot/spot_triangulated.obj");

  // Set up the framebuffer object
  setupFBO();

  renderProg.use();
  renderProg.setUniform("LightIntensity", vec3(0.85f));

  // Set up a  VAO for the full-screen quad
  GLfloat verts[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
  GLuint bufHandle;
  glGenBuffers(1, &bufHandle);
  glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

  // Set up the vertex array object
  glGenVertexArrays( 1, &fsQuad );
  glBindVertexArray(fsQuad);

  glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray(0);  // Vertex position

  glBindVertexArray(0);

  //Load textures
  glActiveTexture(GL_TEXTURE2);
  spotTex = TGAIO::loadTex("../media/spot/spot_texture.tga");
  brickTex = TGAIO::loadTex("../media/texture/brick1.tga");

  updateLight();
  this->animate(true);
}

void SceneShadowVolume::updateLight() {
  lightPos = vec4(5.0f * vec3(cosf(angle) * 7.5f, 1.5f, sinf(angle) * 7.5f), 1.0f);  // World coords
}

// The FBO has one texture for the diffuse + specular
// components.
void SceneShadowVolume::setupFBO()
{
    // The depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf );
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // The ambient buffer
    GLuint ambBuf;
    glGenRenderbuffers(1, &ambBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, ambBuf );
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);

    // The diffuse+specular component
    glActiveTexture(GL_TEXTURE0);
    GLuint diffSpecTex;
    glGenTextures(1, &diffSpecTex);
    glBindTexture(GL_TEXTURE_2D, diffSpecTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create and set up the FBO
    glGenFramebuffers(1, &colorDepthFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ambBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffSpecTex, 0);

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    } else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void SceneShadowVolume::update( float t )
{
  float deltaT = t - tPrev;
  if(tPrev == 0.0f) deltaT = 0.0f;
  tPrev = t;
  if( animating() ) {
    angle += deltaT * rotSpeed;
    if( angle > 2 * PI ) angle -= 2.0 * PI;
    updateLight();
  }
}

void SceneShadowVolume::render()
{
  pass1();
  pass2();
  pass3();
}

// Just renders the geometry normally with shading.  The ambient component
// is rendered to one buffer, and the diffuse and specular componenets are
// written to a texture.
void SceneShadowVolume::pass1() {
  glDepthMask(GL_TRUE);
  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);
  projection = glm::infinitePerspective(50.0f, (float)width/height, 0.5f );
  view = glm::lookAt(vec3(5.0f, 5.0f, 5.0f), vec3(0,2,0), vec3(0,1,0) );

  renderProg.use();
  renderProg.setUniform("LightPosition", view * lightPos);

  glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  drawScene(renderProg, false);
}

// This is the pass that generates the shadow volumes using the
// geometry shader
void SceneShadowVolume::pass2() {
  volumeProg.use();
  volumeProg.setUniform("LightPosition", view * lightPos);

  // Copy the depth and color buffers from the FBO into the default FBO
  // The color buffer should contain the ambient component.
  glBindFramebuffer(GL_READ_FRAMEBUFFER, colorDepthFBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
  glBlitFramebuffer(0,0,width-1,height-1,0,0,width-1,height-1,GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT,GL_NEAREST);

  // Disable writing to the color buffer and depth buffer
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glDepthMask(GL_FALSE);

  // Re-bind to the default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Set up the stencil test so that it always succeeds, increments
  // for front faces, and decrements for back faces.
  glClear(GL_STENCIL_BUFFER_BIT);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, 0, 0xffff);
  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);

  // Draw only the shadow casters
  drawScene(volumeProg, true);

  // Enable writing to the color buffer
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// In this pass, we read the diffuse and specular component from a texture
// and combine it with the ambient component if the stencil test succeeds.
void SceneShadowVolume::pass3() {
  // We don't need the depth test
  glDisable(GL_DEPTH_TEST);

  // We want to just sum the ambient component and the diffuse + specular
  // when the stencil test succeeds, so we'll use this simple blend function.
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE);

  // We want to only render those pixels that have a stencil value
  // equal to zero.
  glStencilFunc(GL_EQUAL, 0, 0xffff);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

  compProg.use();

  // Just draw a screen filling quad
  model = mat4(1.0f);
  projection = model;
  view = model;
  setMatrices(compProg);

  glBindVertexArray(fsQuad);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);

  // Restore some state
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void SceneShadowVolume::drawScene(GLSLProgram &prog, bool onlyShadowCasters)
{
  vec3 color;

  if( ! onlyShadowCasters ) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, spotTex);
    color = vec3(1.0f);
    prog.setUniform("Ka", color * 0.1f);
    prog.setUniform("Kd", color);
    prog.setUniform("Ks", vec3(0.9f));
    prog.setUniform("Shininess", 150.0f);
  }

  model = mat4(1.0f);
  model *= glm::translate(vec3(-2.3f,1.0f,0.2f));
  model *= glm::rotate(180.0f, vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  model = mat4(1.0f);
  model *= glm::translate(vec3(2.5f,1.0f,-1.2f));
  model *= glm::rotate(180.0f, vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  model = mat4(1.0f);
  model *= glm::translate(vec3(0.5f,1.0f,2.7f));
  model *= glm::rotate(180.0f, vec3(0.0f,1.0f,0.0f));
  model = glm::scale(model, vec3(1.5f));
  setMatrices(prog);
  spot->render();

  if( !onlyShadowCasters ) {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brickTex);
    color = vec3(0.5f);
    prog.setUniform("Kd", color);
    prog.setUniform("Ks", vec3(0.0f));
    prog.setUniform("Ka", vec3(0.1f));
    prog.setUniform("Shininess", 1.0f);
    model = mat4(1.0f);
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
    model *= glm::translate(vec3(-5.0f,5.0f,0.0f));
    model *= glm::rotate(90.0f, vec3(1,0,0));
    model *= glm::rotate(-90.0f,vec3(0.0f,0.0f,1.0f));
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,5.0f,-5.0f));
    model *= glm::rotate(90.0f,vec3(1.0f,0.0f,0.0f));
    setMatrices(prog);
    plane->render();
    model = mat4(1.0f);
  }
}

void SceneShadowVolume::setMatrices(GLSLProgram &prog)
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("ProjMatrix", projection);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
}

void SceneShadowVolume::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneShadowVolume::compileAndLinkShader()
{
    try {
      // The shader for the volumes
      volumeProg.compileShader("shader/shadowvolume-vol.vs");
      volumeProg.compileShader("shader/shadowvolume-vol.gs");
      volumeProg.compileShader("shader/shadowvolume-vol.fs");
      volumeProg.link();

      // The shader for rendering and compositing
      renderProg.compileShader("shader/shadowvolume-render.vs");
      renderProg.compileShader("shader/shadowvolume-render.fs");
      renderProg.link();

      // The final composite shader
      compProg.compileShader("shader/shadowvolume-comp.vs");
      compProg.compileShader("shader/shadowvolume-comp.fs");
      compProg.link();

    } catch(GLSLProgramException &e ) {
        cerr << e.what() << endl;
        exit( EXIT_FAILURE );
    }
}
*/

#include "CRenderPass.h"
#include "CMaterial.h"


// Qt
#include <QCoreApplication>

//-----------------------------------------------------------------------------------------
CRenderPass::CRenderPass(
        CMaterial* pMaterial,
        EnumTarget eTarget /*= eScreen*/,
        const QSize& size /*= QSize()*/,
        const CRenderStates& states /*= CRenderStates()*/)
    : AEntity("RenderingPass")
    , m_uiClearBufferFlags(0)
    , m_pMaterial(pMaterial)
    , m_RenderingStates(states)
    , m_ShaderName(DefaultShaderName)
    , m_eTarget(eTarget)
    , m_Size(size)
    , m_pFrameBuffer(0)
    , m_pCamera(0)
    , m_bSaveBitmapEnabled(false)
    , m_SaveBitmapFilePathName(QCoreApplication::applicationDirPath())
{
    if (m_eTarget == eTargetTexture)
    {
        m_pFrameBuffer = new CFrameBuffer(m_Size.width(), m_Size.height());
    }
}

//-----------------------------------------------------------------------------------------
CRenderPass::~CRenderPass()
{
    delete m_pFrameBuffer;
}

//-----------------------------------------------------------------------------------------
int CRenderPass::getIndex() const 
{
    int iCount = 0;

    for (CRenderPass* pPass : m_pMaterial->renderingPassList())
    {
        if (pPass == this)
            return iCount;

        iCount++;
    }

    return -1;
}

//-----------------------------------------------------------------------------------------
const CRenderStates& CRenderPass::getRenderStates() const
{ 
    return m_RenderingStates;
}

//-----------------------------------------------------------------------------------------
CRenderStates& CRenderPass::renderStates()
{ 
    return m_RenderingStates;
}

//--------------------------------------------------------------------------
const QString& CRenderPass::getShaderName() const
{
    return m_ShaderName;
}

//--------------------------------------------------------------------------
bool CRenderPass::isSaveBitmapEnabled() const
{
    return m_bSaveBitmapEnabled;
}

//--------------------------------------------------------------------------
QString CRenderPass::getSaveBitmapFilePathName() const
{
    return m_SaveBitmapFilePathName;
}

//--------------------------------------------------------------------------
void CRenderPass::setShaderName(const QString& shaderName)
{
    m_ShaderName = shaderName;
    m_pMaterial->notifyUpdate();
}

//--------------------------------------------------------------------------
void CRenderPass::setCamera(CCamera* pCamera)
{
    m_pCamera = pCamera;
}

//--------------------------------------------------------------------------
CRenderPass* CRenderPass::previousPass()
{
    return m_pMaterial->getRenderPass(getIndex() - 1);
}

//--------------------------------------------------------------------------
CRenderPass* CRenderPass::nextPass()
{
    return m_pMaterial->getRenderPass(getIndex() + 1);
}

//--------------------------------------------------------------------------
const CRenderPass* CRenderPass::previousPass() const
{
    return m_pMaterial->getRenderPass(getIndex() - 1);
}

//--------------------------------------------------------------------------
const CRenderPass* CRenderPass::nextPass() const
{
    return m_pMaterial->getRenderPass(getIndex() + 1);
}

//--------------------------------------------------------------------------
void CRenderPass::setSize(const QSize& size)
{
    if (size != m_Size)
    {
        m_Size = size;

        if (m_eTarget == eTargetTexture)
        {
            delete m_pFrameBuffer;
            m_pFrameBuffer = new CFrameBuffer(m_Size.width(), m_Size.height());
        }
    }
}

//--------------------------------------------------------------------------
void CRenderPass::setTargetType(EnumTarget eTarget)
{
    if (m_eTarget != eTarget)
    {
        m_eTarget = eTarget;

        if (m_eTarget == eTargetTexture)
        {
            m_pFrameBuffer = new CFrameBuffer(m_Size.width(), m_Size.height());
        }
        else
        {
            delete m_pFrameBuffer;
            m_pFrameBuffer = 0;
        }
    }
}

//--------------------------------------------------------------------------
void CRenderPass::setSaveBitmapEnabled(bool bEnabled)
{
    m_bSaveBitmapEnabled = bEnabled;
}

//--------------------------------------------------------------------------
void CRenderPass::setSaveBitmapFilePathName(const QString& filePathName)
{
    m_SaveBitmapFilePathName = filePathName;
}

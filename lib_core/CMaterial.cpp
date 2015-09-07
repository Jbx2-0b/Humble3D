#include "CMaterial.h"
#include "CLogManager.h"
#include "CGeometryGlobal.h"


//--------------------------------------------------------------------------
CMaterial::CMaterial(const QString& name)
    : AEntity(name)
    , m_AmbientColor(DefaultMaterialAmbientColor)
    , m_DiffuseColor(DefaultMaterialDiffuseColor)
    , m_SpecularColor(DefaultMaterialSpecularColor)
    , m_AlphaMaskColor(DefaultMaterialAlphaMaskColor)
    , m_dShininessFactor(DefaultMaterialShininessFactor)
    , m_dOpacity(1.)
    , m_eAlphaMaskFunc(eAlphaMaskAlways)
{
    m_RenderingPassList << new CRenderPass(this);
}

//--------------------------------------------------------------------------
CMaterial::~CMaterial()
{
    notifyDelete();

    foreach (CRenderPass* pPass, m_RenderingPassList)
    {
        delete pPass;
    }
}

//--------------------------------------------------------------------------
CRenderPass* CMaterial::createRenderPass(
        EnumTarget eTarget,
        const QSize& size /*= QSize(1024, 1024)*/,
        const CRenderStates& states /*= CRenderStates()*/)
{
    CRenderPass* pPass = new CRenderPass(this, eTarget, size, states);
    m_RenderingPassList  << pPass;

    return pPass;
}

//--------------------------------------------------------------------------
void CMaterial::removeRenderingPass(int iIndex)
{
    if (m_RenderingPassList.size() > iIndex)
    {
        CRenderPass* pPass = m_RenderingPassList[iIndex];
        m_RenderingPassList.removeAt(iIndex);
        delete pPass;
    }
}

//--------------------------------------------------------------------------
void CMaterial::removeRenderingPass(CRenderPass* pPass)
{
    if (m_RenderingPassList.contains(pPass))
    {
        m_RenderingPassList.removeOne(pPass);
        delete pPass;
    }
}

//--------------------------------------------------------------------------
CRenderPass* CMaterial::getRenderPass(int iIndex)
{
    if (m_RenderingPassList.size() > iIndex && iIndex >= 0)
    {
        return m_RenderingPassList[iIndex];
    }

    return 0;
}

//--------------------------------------------------------------------------
const CRenderPass* CMaterial::getRenderPass(int iIndex) const
{
    if (m_RenderingPassList.size() > iIndex)
    {
        return m_RenderingPassList[iIndex];
    }

    return 0;
}

//--------------------------------------------------------------------------
const QList<CRenderPass*> CMaterial::getRenderPasses() const
{
    return m_RenderingPassList;
}

//--------------------------------------------------------------------------
void CMaterial::registerListener(IMaterialListener* pListener)
{
    m_MaterialListeners.insert(pListener);
}

//--------------------------------------------------------------------------
void CMaterial::unregisterListener(IMaterialListener* pListener)
{
    m_MaterialListeners.remove(pListener);
}

//--------------------------------------------------------------------------
void CMaterial::notifyDelete()
{
    foreach (IMaterialListener* pListener, m_MaterialListeners)
    {
        pListener->onDelete(this);
    }

    AEntity::notifyDelete();
}

//--------------------------------------------------------------------------
void CMaterial::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        foreach (IMaterialListener* pListener, m_MaterialListeners)
        {
            pListener->onUpdate(this);
        }

        AEntity::notifyUpdate();
    }
}

//--------------------------------------------------------------------------
void CMaterial::addTexture(ATexture* pTexture, EnumMaterialParameter eMaterialParameter)
{
    addTexture(pTexture->getName(), eMaterialParameter);
}

//--------------------------------------------------------------------------
void CMaterial::addTexture(const QString& textureName, EnumMaterialParameter eMaterialParameter)
{
    pLog->addMessage(eINFO, "Material " + getName() + ": Add " + CGeometryGlobal::stringFromMaterialParameter(eMaterialParameter) + " texture : " + textureName);
    if (m_Textures.size() < (int)DefaultMaxTextureChannelCount)
    {
        m_Textures << CTextureParam(textureName, eMaterialParameter);
        notifyUpdate();
    }
    else
    {
        pLog->addMessage(eERROR, "CMaterial::addTexture() : Le maximum de texture par channel est de : " + DefaultMaxTextureChannelCount);
    }
}

//--------------------------------------------------------------------------
void CMaterial::removeTexture(const QString& textureName)
{
    foreach (const CTextureParam& texture, m_Textures)
    {
        if (texture.getTextureName() == textureName)
        {
            m_Textures.removeOne(texture);
        }
    }
}

//--------------------------------------------------------------------------
void CMaterial::setAmbientColor(const QVector4D& color)
{
    m_AmbientColor = color;
}

//--------------------------------------------------------------------------
void CMaterial::setAmbientColor(real r, real g, real b, real a /*= 1.0*/)
{
    m_AmbientColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CMaterial::setDiffuseColor(const QVector4D& color)
{
    m_DiffuseColor = color;
}

//--------------------------------------------------------------------------
void CMaterial::setDiffuseColor(real r, real g, real b, real a /*= 1.0*/)
{
    m_DiffuseColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CMaterial::setSpecularColor(const QVector4D& color)
{
    m_SpecularColor = color;
}

//--------------------------------------------------------------------------
void CMaterial::setSpecularColor(real r, real g, real b, real a /*= 1.0*/)
{
    m_SpecularColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CMaterial::setAlphaMaskColor(const QVector4D& color)
{
    m_AlphaMaskColor = color;
}

//--------------------------------------------------------------------------
void CMaterial::setAlphaMaskColor(real r, real g, real b, real a /*= 1.0*/)
{
    m_AlphaMaskColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CMaterial::setAlphaMaskFunc(EnumAlphaMaskFunc eAlphaMaskFunc)
{
    m_eAlphaMaskFunc = eAlphaMaskFunc;
}

//--------------------------------------------------------------------------
void CMaterial::setShininessFactor(real shininess)
{
    m_dShininessFactor = shininess;
}

//--------------------------------------------------------------------------
void CMaterial::setOpacity(real dOpacity)
{
    m_dOpacity = dOpacity;
}

//-------------------------------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------------------------------
QVector4D CMaterial::getAmbientColor() const
{
    return m_AmbientColor;
}

//--------------------------------------------------------------------------
QVector4D CMaterial::getDiffuseColor() const
{
    return m_DiffuseColor;
}

//--------------------------------------------------------------------------
QVector4D CMaterial::getSpecularColor() const
{
    return m_SpecularColor;
}

//--------------------------------------------------------------------------
QVector4D CMaterial::getAlphaMaskColor() const
{
    return m_AlphaMaskColor;
}

//--------------------------------------------------------------------------
EnumAlphaMaskFunc CMaterial::getAlphaMaskFunc() const
{
    return m_eAlphaMaskFunc;
}

//--------------------------------------------------------------------------
real CMaterial::getShininessFactor() const
{
    return m_dShininessFactor;
}

//--------------------------------------------------------------------------
bool CMaterial::isTextured() const
{	
    return m_Textures.size() > 0;
}

//--------------------------------------------------------------------------
real CMaterial::getOpacity() const
{
    return m_dOpacity;
}


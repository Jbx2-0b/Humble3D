#include "CSoftwareRenderer.h"
#include "CMaterialManager.h"

//-----------------------------------------------------------------------------------------
CSoftwareRenderer::CSoftwareRenderer(CSceneManager* pSceneManager)
    : ARenderer(pSceneManager)
    , m_Rasterizer(DefaultResolutionWidth, DefaultResolutionHeight, Color(Color::eBlack).toVector4D())
    , m_ScreenBBox(0, 0, DefaultResolutionWidth, DefaultResolutionHeight)
    , m_pCurrentMaterial(0)
{
    setName("Software Renderer");

    m_pSceneManager->registerListener(this);
}

//-----------------------------------------------------------------------------------------
CSoftwareRenderer::~CSoftwareRenderer()
{
    m_pSceneManager->unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::setResolution(int iWidth, int iHeight)
{
    m_Rasterizer.setResolution(iWidth, iHeight);
    m_ScreenBBox = CBox2D(0, 0, iWidth, iHeight);

    ARenderer::setResolution(iWidth, iHeight);
}

//-----------------------------------------------------------------------------------------
// Implémentation ARenderer
//-----------------------------------------------------------------------------------------
bool CSoftwareRenderer::init()
{
    return ARenderer::init();
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::render()
{
    if (!m_pSceneManager)
        return;

    if (m_bEnabled && isDirty())
    {
        wash();

        m_Rasterizer.clear();

        if (!m_pCurrentCamera)
        {
            return;
        }

        const CRenderQueue& renderQueue = m_pSceneManager->getRenderQueue();
        QList<int> materialIDs = renderQueue.materialIDs();


        foreach (int materialID, materialIDs)
        {

            if (CMaterial* pMaterial = CMaterialManager::getInstance().getMaterialByID(materialID))
            {
                m_pCurrentMaterial = pMaterial;

                const QList<ARenderableItem*>& items = renderQueue.getItemsByMaterialID(materialID);

                foreach (ARenderableItem* pItem, items)
                {
                    if (pItem->isVisible())
                    {
                        foreach (CSceneNode* pNode, pItem->getNodes())
                        {
                            // On concaténe les matrices pour obtenir la matrice "Model-Vue-Projection" qui correspond
                            // au changement de repére pour passer du répére de l'objet à celui de la caméra
                            m_ModelViewProjectionMatrix = m_pCurrentCamera->getTransformation() * pNode->getGlobalTransformation();

                            pItem->doRender(this);
                        }
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::renderMeshBuffer(CMeshBuffer* pBuffer)
{
    const CBuffer<IndiceType>& indiceBuffer = pBuffer->indicesBuffer();
    const CBuffer<QVector3D>& posBuffer = pBuffer->positionsBuffer();
    //const CBuffer<QVector3D>& normalBuffer = pBuffer->normalsBuffer();

    static QVector3D pos2D;
    static QVector3D pt[3];

    double dZNear = m_pCurrentCamera->getNear();
    double dZFar = m_pCurrentCamera->getFar();



    double dWidth = (double)getWidth();
    double dHeight = (double)getHeight();

    QVector4D color = m_pCurrentMaterial->getAmbientColor();

    if (pBuffer->getPrimitiveType() == ePrimitiveTriangles)
    {
        //qDebug() << "indiceBuffer.size(): " << indiceBuffer.size();

        int iIndex = 0;

        while (iIndex < indiceBuffer.size())
        {
            for (int i = 0; i < 3; ++i)
            {
                int iPosID = indiceBuffer.at(iIndex + i);
                // On convertit la position XYZ dans le repére local du modéle dans le repére caméra via la matrice vue-projection

                // TODO
                /*
                if (pBuffer->hasSkeleton())
                {
                    QVector<QMatrix4x4> bones = pBuffer->getBoneTransformations();

                    //qDebug() << "iIndex: " << iIndex;
                    //qDebug() << "boneIDsBuffer() size: " << pBuffer->boneIDsBuffer().size();
                    //qDebug() << "boneWeightsBuffer() size: " << pBuffer->boneWeightsBuffer().size();
                    //qDebug() << "boneIDsBuffer()[iIndex]: " << pBuffer->boneIDsBuffer()[iPosID];
                    //qDebug() << "boneWeightsBuffer()[iIndex]: " << pBuffer->boneWeightsBuffer()[iPosID];


                    QMatrix4x4 boneTransform = bones[pBuffer->boneIDsBuffer()[iPosID]] * pBuffer->boneWeightsBuffer()[iPosID];


                    //for (int b = 1; b < DefaultMaxBonesPerVertex; ++b)
                    //{
                    //	boneTransform += bones[pBuffer->boneIDsBuffer()[iPosID][b]] * pBuffer->boneWeightsBuffer()[iPosID][b];
                    //}

                    pos2D = m_ModelViewProjectionMatrix * boneTransform * posBuffer.at(iPosID);
                }
                else*/
                {
                    pos2D = m_ModelViewProjectionMatrix * posBuffer.at(iPosID);
                }

                // Frustum culling
                if (pos2D.z() < dZNear || pos2D.z() > dZFar) break;

                // On convertit les données normalisées dans la surface de l'écran [-1; 1]
                // En coordonnées XY classique exprimé depuis le coin supérieur gauche et tenant compte de la résolution de l'image
                pt[i].setX(Math::round(((pos2D.x() + 1.0) / 2.0) * dWidth));
                pt[i].setY(Math::round(((1.0 - pos2D.y()) / 2.0) * dHeight));
                pt[i].setZ(pos2D.z());
            }

            iIndex += 3;


            if ((QVector3D::normal(pt[0], pt[1], pt[2]).z() < 0.)  &&								// back face culling
                    isInsideScreen(pt[0].toVector2D(), pt[1].toVector2D(), pt[2].toVector2D()))			// clipping
            {
                const real c[][4] = {{color.x(), color.y(), color.z(), color.w()}, {color.x(), color.y(), color.z(), color.w()}, {color.x(), color.y(), color.z(), color.w()}};
                const real p[][3] = {{pt[0].x(), pt[0].y(), pt[0].z()}, {pt[1].x(), pt[1].y(), pt[1].z()}, {pt[2].x(), pt[2].y(), pt[2].z()}};

                m_Rasterizer.drawTriangle(c, p);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::renderMeshBuffer(const QList<CMeshBuffer*>& buffers)
{
    foreach (CMeshBuffer* pBuffer, buffers)
    {
        renderMeshBuffer(pBuffer);
    }
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::onUpdateRenderStates()
{
    // TODO
}

//-----------------------------------------------------------------------------------------
// Implémentation CSceneManager::ISceneManagerListener
//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::onUpdateCamera(CCamera* /*pCamera*/)
{
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CSoftwareRenderer::onUpdateAnimation(CAnimation* /*pAnimation*/)
{
    setDirty();
}


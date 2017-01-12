#include "CNodeTreeWidget.h"
#include "CQtHelper.h"
#include "CShader.h"
#include "CLogManager.h"
#include "CTextureManager.h"
#include "CEditShaderTextItem.h"
#include "CParamShaderItem.h"
#include "CEditShaderTreeItem.h"
#include "CVectorWidgetItem.h"
#include "CComboBoxItem.h"
#include "CSaveDialog.h"
#include "CAnimationTreeItem.h"

// Qt
#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>
#include <QFileDialog>

static const QString AmbientColorKey = "Ambient Color";
static const QString DiffuseColorKey = "Diffuse Color";
static const QString SpecularColorKey = "Specular Color";
static const QString AlphaMaskColorKey = "Alpha Mask Color";
static const QString AlphaMaskFuncKey = "Alpha Mask Func";
static const QString ShininessFactorKey = "Shininess Factor";
static const QString OpacityKey = "Opacity";
static const QString BlendingKey = "Blending";
static const QString WireframeKey = "Wireframe";
static const QString TexturedKey = "Textured";
static const QString ShaderNameKey = "Shader Name";
static const QString TextureTypeKey = "Texture Type";
static const QString MatParamKey = "Mat Param";
static const QString RenderingTargetKey = "Rendering Target";
static const QString ClearColorBufferKey = "Clear Color Buffer";
static const QString ClearDepthBufferKey = "Clear Depth Buffer";
static const QString ClearStencilBufferKey = "Clear Stencil Buffer";
static const QString ClearColorKey = "Clear Color";
static const QString EnabledKey = "Enabled";
static const QString SourceTypeKey = "Source Type";
static const QString PositionKey = "Position";
static const QString RotationKey = "Rotation";
static const QString ScaleKey = "Scale";
static const QString DirectionKey = "Direction";
static const QString ConstantAttenuationKey = "Constant Attenuation";
static const QString LinearAttenuationKey = "Linear Attenuation";
static const QString QuadraticAttenuationKey = "Quadratic Attenuation";
static const QString InnerConeKey = "Inner Cone";
static const QString OuterConeKey = "Outer Cone";
static const QString EyePositionKey = "Eye Position";
static const QString CenterKey = "Center";
static const QString ForwardKey = "Forward";
static const QString UpKey = "Up";
static const QString HorizontalFOVKey = "Horizontal FOV";
static const QString ClipPlaneNearKey = "Clip Plane Near";
static const QString ClipPlaneFarKey = "Clip Plane Far";
static const QString AspectRatioKey = "Aspect Ratio";
static const QString ProjectionTypeKey = "Projection Type";
static const QString DurationKey = "Duration";
static const QString TicksPerSecKey = "Ticks/s";
static const QString StateBehaviorKey = "State Behavior";
static const QString BonesCountKey = "Bones Count";
static const QString TimeKey = "Time";
static const QString ValueKey = "Value";
static const QString SceneGraphKey = "Scene Graph";
static const QString RasterizationModeKey = "Rasterization Mode";
static const QString VisibleKey = "Visible";
static const QString SelectableKey = "Selectable";
static const QString SelectedKey = "Selected";
static const QString MaterialKey = "Material";
static const QString NormalsVisibleKey = "Normals Visible";
static const QString PrimitivesKey = "Primitives";
static const QString PositionsCountKey = "Positions Count";
static const QString IndicesCountKey = "Indices Count";
static const QString BoneCountKey = "Bones Count";
static const QString NameKey = "Name";
static const QString FileNameKey = "FileName";
static const QString FileNamesKey = "FileNames";
static const QString VertexShaderKey = "Vertex Shader";
static const QString FragmentShaderKey = "Fragment Shader";
static const QString GeometryShaderKey = "Geometry Shader";

//-----------------------------------------------------------------------------------------
CNodeTreeWidget::CNodeTreeWidget(CSceneManager* pSceneManager, AView* pView, CEditShaderTextItem* pEditItem, QWidget* parent /*= 0*/)
    : QTreeWidget(parent)
    , m_pSceneManager(pSceneManager)
    , m_pView(pView)
    , m_pCurrentEntity(0)
    , m_pEditItem(pEditItem)
{
    setColumnCount(1);
    header()->setVisible(false);

    updateData();

    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    setExpandsOnDoubleClick(false);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onItemChanged(QTreeWidgetItem*, int)));

    m_pDeleteAction = new QAction(tr("Delete"), this);
    connect(m_pDeleteAction, SIGNAL(triggered()), this, SLOT(onDeleteClicked()));

    m_pSaveAction = new QAction(tr("Save"), this);
    connect(m_pSaveAction, SIGNAL(triggered()), this, SLOT(onSaveClicked()));

    m_pSetCurrentAction = new QAction(tr("Set Current"), this);
    connect(m_pSetCurrentAction, SIGNAL(triggered()), this, SLOT(onSetCurrentClicked()));

    QTimer* pUpdateTimer = new QTimer(this);
    connect(pUpdateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    pUpdateTimer->start(200);
}

//-----------------------------------------------------------------------------------------
CNodeTreeWidget::~CNodeTreeWidget()
{
    foreach (AEntity* pEntity, m_Entities)
    {
        pEntity->unregisterListener(this);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::clearData()
{
    foreach (AEntity* pEntity, m_Entities)
    {
        pEntity->unregisterListener(this);
    }

    m_Entities.clear();
    m_PropertiesItems.clear();

    clear();
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateData()
{
    LogManager.addMessage(eDEBUG, "NodeTreeWidget::updateData()");

    clearData();

    // On bloque les signaux pour éviter de boucler sur les événements Listener
    blockSignals(true);
    createTreeSceneManager();
    createTreeRenderer();
    recursiveCreateTreeNodes(m_pSceneManager->getRootNode());
    //createTreeMeshs(CMeshManager::getInstance().getMeshs());
    createTreeLights(m_pSceneManager->getLights());
    createTreeCameras(m_pSceneManager->getCameras());
    createTreeMaterials(CMaterialManager::getInstance().getMaterials());
    createTreeAnimations(m_pSceneManager->getAnimations());
    createTreeShaders(CShaderManager::getInstance().getShaders());
    blockSignals(false);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::contextMenuEvent(QContextMenuEvent* pEvent)
{
    QList<QTreeWidgetItem*> items = selectedItems();

    if (items.count() > 0)
    {
        QTreeWidgetItem* pCurrentItem = items[0];

        if (AEntity* pEntity = m_Entities.value(pCurrentItem))
        {
            m_pCurrentEntity = pEntity;

            QMenu menu(this);

            if (ASceneNodeItem* pNodeItem = dynamic_cast<ASceneNodeItem*>(pEntity))
            {
                menu.addAction(m_pSaveAction);
                menu.addAction(m_pDeleteAction);

                if (dynamic_cast<CCamera*>(pNodeItem))
                {
                    menu.addAction(m_pSetCurrentAction);
                }
            }

            menu.exec(pEvent->globalPos());
        }
    }
    QTreeWidget::contextMenuEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
// LISTENERS
//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onUpdate(AEntity* pEntity)
{
    if (m_Entities.key(pEntity, 0))
    {
        pEntity->unregisterListener(this);

        if (CMaterial* pMaterial = dynamic_cast<CMaterial*>(pEntity))
        {
            updateProperty(pMaterial, AmbientColorKey,			pMaterial->getAmbientColor());
            updateProperty(pMaterial, DiffuseColorKey,			pMaterial->getDiffuseColor());
            updateProperty(pMaterial, SpecularColorKey,			pMaterial->getSpecularColor());
            updateProperty(pMaterial, AlphaMaskColorKey,		pMaterial->getAlphaMaskColor());
            updateProperty(pMaterial, AlphaMaskFuncKey,			CGeometryGlobal::stringFromAlphaMaskFunc(pMaterial->getAlphaMaskFunc()));
            updateProperty(pMaterial, ShininessFactorKey,		QString::number(pMaterial->getShininessFactor()));
            updateProperty(pMaterial, OpacityKey,				QString::number(pMaterial->getOpacity()));
            updateProperty(pMaterial, TexturedKey,				pMaterial->isTextured());

            foreach (const CTextureParam& texture, pMaterial->getTextureParams())
            {
                ATexture* pTexture = CTextureManager::getInstance().getTextureByName(texture.getTextureName());

                updateProperty(pTexture, TextureTypeKey,	CGeometryGlobal::stringFromTextureType(pTexture->getType()));
                updateProperty(pTexture, MatParamKey,		CGeometryGlobal::stringFromMaterialParameter(texture.getMaterialParameter()));

                if (CTexture2D* pTexture2D = dynamic_cast<CTexture2D*>(pTexture))
                {
                    updateProperty(pTexture, FileNameKey, pTexture2D->getFileName());
                }
                else if (CTextureCube* pTextureCube = dynamic_cast<CTextureCube*>(pTexture))
                {
                    // TODO
                    // updateProperty(pTexture, pTDWidgetItem, "FileNames",		pTextureCube->getFileNames());
                }
            }
        }
        else if (CRenderPass* pPass =  dynamic_cast<CRenderPass*>(pEntity))
        {
            updateProperty(pPass, ShaderNameKey,	pPass->getShaderName());
            updateProperty(pPass, RenderingTargetKey,	CGeometryGlobal::stringFromTargetType(pPass->getTargetType()));
            updateProperty(pPass, ClearColorKey,		pPass->getClearColor());
        }
        else if (CLight* pLight = dynamic_cast<CLight*>(pEntity))
        {
            updateProperty(pLight, EnabledKey,				pLight->isEnabled());
            updateProperty(pLight, SourceTypeKey,			CGeometryGlobal::stringFromLightSourceType(pLight->getSourceType()));
            updateProperty(pLight, DirectionKey,			pLight->getDirection());
            updateProperty(pLight, ConstantAttenuationKey,	QString::number(pLight->getConstantAttenuation()));
            updateProperty(pLight, LinearAttenuationKey,	QString::number(pLight->getLinearAttenuation()));
            updateProperty(pLight, QuadraticAttenuationKey,	QString::number(pLight->getQuadraticAttenuation()));
            updateProperty(pLight, AmbientColorKey,			pLight->getAmbientColor());
            updateProperty(pLight, DiffuseColorKey,			pLight->getDiffuseColor());
            updateProperty(pLight, SpecularColorKey,		pLight->getSpecularColor());
            updateProperty(pLight, InnerConeKey,			QString::number(pLight->getInnerConeAngle()));
            updateProperty(pLight, OuterConeKey,			QString::number(pLight->getOuterConeAngle()));
        }
        else if (CCamera* pCamera = dynamic_cast<CCamera*>(pEntity))
        {
            updateProperty(pCamera, EyePositionKey,		pCamera->getEyePosition());
            updateProperty(pCamera, CenterKey,			pCamera->getCenter());
            updateProperty(pCamera, ForwardKey,			pCamera->getForward());
            updateProperty(pCamera, UpKey,				pCamera->getUp());
            updateProperty(pCamera, HorizontalFOVKey,	QString::number(pCamera->getHorizontalFOV()));
            updateProperty(pCamera, ClipPlaneNearKey,	QString::number(pCamera->getNear()));
            updateProperty(pCamera, ClipPlaneFarKey,	QString::number(pCamera->getFar()));
            updateProperty(pCamera, AspectRatioKey,		QString::number(pCamera->getAspectRatio()));
            updateProperty(pCamera, ProjectionTypeKey,	CGeometryGlobal::stringFromProjectionType(pCamera->getProjectionType()));
        }
        else if (CAnimation* pAnimation = dynamic_cast<CAnimation*>(pEntity))
        {
            updateProperty(pAnimation, DurationKey,		QString::number(pAnimation->getDuration()));
            updateProperty(pAnimation, TicksPerSecKey,	QString::number(pAnimation->getTicksPerSecond()));

            foreach (CSceneNodeAnimation* pNA, pAnimation->getNodeAnimations())
            {
                updateProperty(pNA, StateBehaviorKey, CGeometryGlobal::stringFromAnimationBehaviorType(pNA->getAnimationBehavior()));


                foreach (const CVectorKey& vK, pNA->positionKeys())
                {
                    updateProperty(pNA, TimeKey,		QString::number(vK.getTime()));
                    updateProperty(pNA, ValueKey,		vK.getValue());
                }

                foreach (const CQuaternionKey& qK, pNA->rotationKeys())
                {
                    updateProperty(pNA, TimeKey,		QString::number(qK.getTime()));
                }

                foreach (const CVectorKey& vK, pNA->scalingKeys())
                {
                    updateProperty(pNA, TimeKey,		QString::number(vK.getTime()));
                    updateProperty(pNA, ValueKey,		vK.getValue());
                }
            }
        }
        else if (CShader* pShader = dynamic_cast<CShader*>(pEntity))
        {
            foreach (const TUniformValue& uniformValue, pShader->getUniformValues())
            {
                switch (int(uniformValue.second.type()))
                {
                case QMetaType::Int:
                    updateProperty(pShader, uniformValue.first, QString::number(uniformValue.second.value<int>()));
                    break;
                case QMetaType::Float:
                    updateProperty(pShader, uniformValue.first, QString::number(uniformValue.second.value<float>()));
                    break;
                case QMetaType::Double:
                    updateProperty(pShader, uniformValue.first, QString::number(uniformValue.second.value<double>()));
                    break;
                case QMetaType::QVector2D:
                    updateProperty(pShader, uniformValue.first, uniformValue.second.value<QVector2D>().toVector3D());
                    break;
                case QMetaType::QVector3D:
                    updateProperty(pShader, uniformValue.first, uniformValue.second.value<QVector3D>());
                    break;
                case QMetaType::QVector4D:
                    updateProperty(pShader, uniformValue.first, uniformValue.second.value<QVector4D>());
                    break;
                case QMetaType::QMatrix4x4:
                    // TODO
                    break;
                default:
                    break;
                }
            }
        }

        pEntity->registerListener(this);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onDelete(AEntity* pEntity)
{
    LogManager.addMessage(eDEBUG, "NodeTreeWidget::onDestroy() : " + pEntity->getName());

    if (QTreeWidgetItem* pItem = m_Entities.key(pEntity, 0))
    {
        m_Entities.remove(pItem);
        delete pItem;
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onUpdate(CMaterial* pMaterial)
{
    foreach (CMeshInstance* pMeshInstance, m_pSceneManager->getMeshInstances())
    {
        if (QTreeWidgetItem* pItem = getPropertyTreeItem(pMeshInstance, MaterialKey))
        {
            addComboBoxEntry(pItem, pMaterial->getName());
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onDelete(CMaterial* pMaterial)
{
    foreach (CMeshInstance* pMeshInstance, m_pSceneManager->getMeshInstances())
    {
        if (QTreeWidgetItem* pItem = getPropertyTreeItem(pMeshInstance, MaterialKey))
        {
            removeComboBoxEntry(pItem, pMaterial->getName());
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::selectEntity(AEntity* pEntity)
{
    if (QTreeWidgetItem* pItem = m_Entities.key(pEntity, 0))
    {
        setCurrentItem(pItem);
        expandItem(pItem);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addEntity(QTreeWidgetItem* pTreeWidgetItem, AEntity* pEntity)
{
    m_Entities[pTreeWidgetItem] = pEntity;
    pEntity->registerListener(this);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeSceneManager()
{
    QTreeWidgetItem* pSMWidgetItem = new QTreeWidgetItem();
    addTopLevelItem(pSMWidgetItem);
    pSMWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    addEntity(pSMWidgetItem, m_pSceneManager);

    pSMWidgetItem->setText(0, m_pSceneManager->getName());
    pSMWidgetItem->setIcon(0, QIcon(":/Resources/Notifications.png"));
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeRenderer()
{
    QTreeWidgetItem* pRendererWidgetItem = new QTreeWidgetItem();
    addTopLevelItem(pRendererWidgetItem);
    pRendererWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    addEntity(pRendererWidgetItem, m_pView->getRenderer());

    pRendererWidgetItem->setText(0, m_pView->getRenderer()->getName());
    pRendererWidgetItem->setIcon(0, QIcon(":/Resources/Renderer.png"));

    addProperty(m_pView->getRenderer(), pRendererWidgetItem, RasterizationModeKey,	CGeometryGlobal::rasterizationModeEntries(), CGeometryGlobal::stringFromRasterizationMode(m_pView->getRenderer()->getRasterizationMode()));
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::recursiveCreateTreeNodes(CSceneNode* pNode, QTreeWidgetItem* pParent /*= 0*/)
{
    QTreeWidgetItem* pNodeWidgetItem = 0;

    if (pParent)
    {
        pNodeWidgetItem = new QTreeWidgetItem();
        pParent->addChild(pNodeWidgetItem);
    }
    else
    {
        pNodeWidgetItem = new QTreeWidgetItem();
        addTopLevelItem(pNodeWidgetItem);
    }

    pNodeWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    addEntity(pNodeWidgetItem, pNode);

    pNodeWidgetItem->setText(0, pNode->getName());
    pNodeWidgetItem->setIcon(0, QIcon(":/Resources/node.png"));

    foreach (ASceneNodeItem* pItem, pNode->getItems())
    {
        QTreeWidgetItem* pNodeItemWidgetItem = new QTreeWidgetItem();
        pNodeItemWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        pNodeWidgetItem->addChild(pNodeItemWidgetItem);
        pNodeItemWidgetItem->setText(0, pItem->getName());
        pNodeItemWidgetItem->setIcon(0, QIcon(":/Resources/warning.png"));

        addEntity(pNodeItemWidgetItem, pItem);

        addProperty(pItem, pNodeItemWidgetItem, VisibleKey,	pItem->isVisible());
        addProperty(pItem, pNodeItemWidgetItem, SelectableKey, pItem->isSelectable());
        addProperty(pItem, pNodeItemWidgetItem, SelectedKey,	pItem->isSelected());

        if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(pItem))
        {
            pNodeItemWidgetItem->setIcon(0, QIcon(":/Resources/geometry.png"));

            foreach (CSubMeshInstance* pSubMeshInstance, pMeshInstance->subMeshInstances())
            {
                QTreeWidgetItem* pSubMeshInstanceWidgetItem = new QTreeWidgetItem();
                pSubMeshInstanceWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

                pNodeItemWidgetItem->addChild(pSubMeshInstanceWidgetItem);
                pSubMeshInstanceWidgetItem->setText(0, pSubMeshInstance->getName());
                pSubMeshInstanceWidgetItem->setIcon(0, QIcon(":/Resources/geometry.png"));

                addProperty(pSubMeshInstance, pSubMeshInstanceWidgetItem, MaterialKey, CMaterialManager::getInstance().getMaterialsNames(), pSubMeshInstance->getMaterialName(), true);
            }
        }
        else if (CLight* pLight = dynamic_cast<CLight*>(pItem))
        {
            pNodeItemWidgetItem->setIcon(0, QIcon(":/Resources/Light.png"));

            addProperty(pLight, pNodeItemWidgetItem, EnabledKey,				pLight->isEnabled());
            addProperty(pLight, pNodeItemWidgetItem, SourceTypeKey,             CGeometryGlobal::lightSourceTypeEntries(), CGeometryGlobal::stringFromLightSourceType(pLight->getSourceType()));
            addProperty(pLight, pNodeItemWidgetItem, DirectionKey,				pLight->getDirection());
            addProperty(pLight, pNodeItemWidgetItem, ConstantAttenuationKey,	QString::number(pLight->getConstantAttenuation()));
            addProperty(pLight, pNodeItemWidgetItem, LinearAttenuationKey,		QString::number(pLight->getLinearAttenuation()));
            addProperty(pLight, pNodeItemWidgetItem, QuadraticAttenuationKey,	QString::number(pLight->getQuadraticAttenuation()));
            addProperty(pLight, pNodeItemWidgetItem, AmbientColorKey,			pLight->getAmbientColor());
            addProperty(pLight, pNodeItemWidgetItem, DiffuseColorKey,			pLight->getDiffuseColor());
            addProperty(pLight, pNodeItemWidgetItem, SpecularColorKey,			pLight->getSpecularColor());
            addProperty(pLight, pNodeItemWidgetItem, InnerConeKey,				QString::number(pLight->getInnerConeAngle()));
            addProperty(pLight, pNodeItemWidgetItem, OuterConeKey,				QString::number(pLight->getOuterConeAngle()));
        }
        else if (CCamera* pCamera = dynamic_cast<CCamera*>(pItem))
        {
            pNodeItemWidgetItem->setIcon(0, QIcon(":/Resources/Camera.png"));

            addProperty(pCamera, pNodeItemWidgetItem, EyePositionKey,		pCamera->getEyePosition());
            addProperty(pCamera, pNodeItemWidgetItem, CenterKey,			pCamera->getCenter());
            addProperty(pCamera, pNodeItemWidgetItem, ForwardKey,           pCamera->getForward(), false);
            addProperty(pCamera, pNodeItemWidgetItem, UpKey,				pCamera->getUp());
            addProperty(pCamera, pNodeItemWidgetItem, HorizontalFOVKey,     QString::number(pCamera->getHorizontalFOV()));
            addProperty(pCamera, pNodeItemWidgetItem, ClipPlaneNearKey,     QString::number(pCamera->getNear()));
            addProperty(pCamera, pNodeItemWidgetItem, ClipPlaneFarKey,      QString::number(pCamera->getFar()));
            addProperty(pCamera, pNodeItemWidgetItem, AspectRatioKey,		QString::number(pCamera->getAspectRatio()));
            addProperty(pCamera, pNodeItemWidgetItem, ProjectionTypeKey,	CGeometryGlobal::projectionTypeEntries(), CGeometryGlobal::stringFromProjectionType(pCamera->getProjectionType()));
        }
    }

    foreach (CSceneNode* pChild, pNode->getChildNodes())
    {
        recursiveCreateTreeNodes(pChild, pNodeWidgetItem);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeMeshs(const QList<CMesh*>& meshs)
{
    foreach (CMesh* pMesh, meshs)
    {
        QTreeWidgetItem* pMeshWidgetItem = new QTreeWidgetItem();
        addTopLevelItem(pMeshWidgetItem);

        pMeshWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pMeshWidgetItem, pMesh);

        pMeshWidgetItem->setText(0, pMesh->getName());
        pMeshWidgetItem->setIcon(0, QIcon(":/Resources/geometry.png"));

        foreach (CSubMesh* pSubMesh, pMesh->subMeshs())
        {
            QTreeWidgetItem* pSubMeshWidgetItem = new QTreeWidgetItem();
            pSubMeshWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            pMeshWidgetItem->addChild(pSubMeshWidgetItem);
            pSubMeshWidgetItem->setText(0, pSubMesh->getName());
            pSubMeshWidgetItem->setIcon(0, QIcon(":/Resources/geometry.png"));

            addProperty(pSubMesh, pSubMeshWidgetItem, MaterialKey,          CMaterialManager::getInstance().getMaterialsNames(), pSubMesh->getMaterialName(), true);

            addProperty(pSubMesh, pSubMeshWidgetItem, PrimitivesKey,		CGeometryGlobal::primitiveTypeEntries(), CGeometryGlobal::stringFromPrimitiveType(pSubMesh->getPrimitiveType()), false);
            addProperty(pSubMesh, pSubMeshWidgetItem, PositionsCountKey,    QString::number(pSubMesh->positionsBuffer().count()), false);
            addProperty(pSubMesh, pSubMeshWidgetItem, IndicesCountKey,      QString::number(pSubMesh->indicesBuffer().count()), false);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeLights(const QList<CLight*>& lights)
{
    foreach (CLight* pLight, lights)
    {
        QTreeWidgetItem* pLightWidgetItem = new QTreeWidgetItem();
        addTopLevelItem(pLightWidgetItem);

        pLightWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pLightWidgetItem, pLight);

        pLightWidgetItem->setText(0, pLight->getName());
        pLightWidgetItem->setIcon(0, QIcon(":/Resources/Light.png"));

        addProperty(pLight, pLightWidgetItem, EnabledKey,				pLight->isEnabled());
        addProperty(pLight, pLightWidgetItem, SourceTypeKey,            CGeometryGlobal::lightSourceTypeEntries(), CGeometryGlobal::stringFromLightSourceType(pLight->getSourceType()));
        addProperty(pLight, pLightWidgetItem, DirectionKey,				pLight->getDirection());
        addProperty(pLight, pLightWidgetItem, ConstantAttenuationKey,	QString::number(pLight->getConstantAttenuation()));
        addProperty(pLight, pLightWidgetItem, LinearAttenuationKey,		QString::number(pLight->getLinearAttenuation()));
        addProperty(pLight, pLightWidgetItem, QuadraticAttenuationKey,	QString::number(pLight->getQuadraticAttenuation()));
        addProperty(pLight, pLightWidgetItem, AmbientColorKey,			pLight->getAmbientColor());
        addProperty(pLight, pLightWidgetItem, DiffuseColorKey,			pLight->getDiffuseColor());
        addProperty(pLight, pLightWidgetItem, SpecularColorKey,			pLight->getSpecularColor());
        addProperty(pLight, pLightWidgetItem, InnerConeKey,				QString::number(pLight->getInnerConeAngle()));
        addProperty(pLight, pLightWidgetItem, OuterConeKey,				QString::number(pLight->getOuterConeAngle()));
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeCameras(const QList<CCamera*>& cameras)
{
    foreach (CCamera* pCamera, cameras)
    {
        QTreeWidgetItem* pCameraWidgetItem = new QTreeWidgetItem();
        addTopLevelItem(pCameraWidgetItem);

        pCameraWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pCameraWidgetItem, pCamera);

        pCameraWidgetItem->setText(0, pCamera->getName());
        pCameraWidgetItem->setIcon(0, QIcon(":/Resources/Camera.png"));

        addProperty(pCamera, pCameraWidgetItem, EyePositionKey,		pCamera->getEyePosition());
        addProperty(pCamera, pCameraWidgetItem, CenterKey,			pCamera->getCenter());
        addProperty(pCamera, pCameraWidgetItem, ForwardKey,         pCamera->getForward(), false);
        addProperty(pCamera, pCameraWidgetItem, UpKey,				pCamera->getUp());
        addProperty(pCamera, pCameraWidgetItem, HorizontalFOVKey,   QString::number(pCamera->getHorizontalFOV()));
        addProperty(pCamera, pCameraWidgetItem, ClipPlaneNearKey,   QString::number(pCamera->getNear()));
        addProperty(pCamera, pCameraWidgetItem, ClipPlaneFarKey,    QString::number(pCamera->getFar()));
        addProperty(pCamera, pCameraWidgetItem, AspectRatioKey,		QString::number(pCamera->getAspectRatio()));
        addProperty(pCamera, pCameraWidgetItem, ProjectionTypeKey,	CGeometryGlobal::projectionTypeEntries(), CGeometryGlobal::stringFromProjectionType(pCamera->getProjectionType()));
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeMaterials(const QList<CMaterial*>& materials)
{
    foreach (CMaterial* pMaterial, materials)
    {
        QTreeWidgetItem* pMaterialWidgetItem = new QTreeWidgetItem();
        addTopLevelItem(pMaterialWidgetItem);

        pMaterialWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pMaterialWidgetItem, pMaterial);

        pMaterialWidgetItem->setText(0, pMaterial->getName());
        pMaterialWidgetItem->setIcon(0, QIcon(":/Resources/Material.png"));

        addProperty(pMaterial, pMaterialWidgetItem, AmbientColorKey,		pMaterial->getAmbientColor());
        addProperty(pMaterial, pMaterialWidgetItem, DiffuseColorKey,		pMaterial->getDiffuseColor());
        addProperty(pMaterial, pMaterialWidgetItem, SpecularColorKey,		pMaterial->getSpecularColor());
        addProperty(pMaterial, pMaterialWidgetItem, AlphaMaskColorKey,		pMaterial->getAlphaMaskColor());
        addProperty(pMaterial, pMaterialWidgetItem, AlphaMaskFuncKey,		CGeometryGlobal::alphaMaskFuncEntries(), CGeometryGlobal::stringFromAlphaMaskFunc(pMaterial->getAlphaMaskFunc()));
        addProperty(pMaterial, pMaterialWidgetItem, ShininessFactorKey,		QString::number(pMaterial->getShininessFactor()));
        addProperty(pMaterial, pMaterialWidgetItem, OpacityKey,				QString::number(pMaterial->getOpacity()));
        addProperty(pMaterial, pMaterialWidgetItem, TexturedKey,			pMaterial->isTextured(), false);

        foreach (const CTextureParam& texture, pMaterial->getTextureParams())
        {
            if (ATexture* pTexture = CTextureManager::getInstance().getTextureByName(texture.getTextureName()))
            {
                QTreeWidgetItem* pTDWidgetItem = new QTreeWidgetItem();
                pTDWidgetItem->setText(0, pTexture->getName());
                pMaterialWidgetItem->addChild(pTDWidgetItem);

                addProperty(pTexture, pTDWidgetItem, TextureTypeKey,	CGeometryGlobal::textureTypeEntries(), CGeometryGlobal::stringFromTextureType(pTexture->getType()), false);
                addProperty(pTexture, pTDWidgetItem, MatParamKey,		CGeometryGlobal::materialParameterEntries(), CGeometryGlobal::stringFromMaterialParameter(texture.getMaterialParameter()), false);

                if (CTexture2D* pTexture2D = dynamic_cast<CTexture2D*>(pTexture))
                {
                    addProperty(pTexture, pTDWidgetItem, FileNameKey, pTexture2D->getFileName());
                }
                else if (CTextureCube* pTextureCube = dynamic_cast<CTextureCube*>(pTexture))
                {
                    // TODO
                    // addProperty(pTexture, pTDWidgetItem, "FileNames",		pTextureCube->getFileNames());
                }
            }
        }

        QTreeWidgetItem* pRenderingPassWidgetItem = new QTreeWidgetItem();
        pRenderingPassWidgetItem->setText(0, "Rendering Pass");
        pMaterialWidgetItem->addChild(pRenderingPassWidgetItem);

        foreach (CRenderPass* pPass, pMaterial->renderingPassList())
        {
            QTreeWidgetItem* pTargetWidgetItem = new QTreeWidgetItem();
            pTargetWidgetItem->setText(0, QString("RenderingPass%1").arg(pPass->getIndex()));
            pRenderingPassWidgetItem->addChild(pTargetWidgetItem);


            addProperty(pPass, pTargetWidgetItem, ShaderNameKey,        pPass->getShaderName());
            addProperty(pPass, pTargetWidgetItem, RenderingTargetKey,	CGeometryGlobal::targetTypeEntries(), CGeometryGlobal::stringFromTargetType(pPass->getTargetType()));
            addProperty(pPass, pTargetWidgetItem, ClearColorKey,		pPass->getClearColor());
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeAnimations(const QList<CAnimation*>& animations)
{
    foreach (CAnimation* pAnimation, animations)
    {
        CAnimationTreeItem* pAnimationWidgetItem = new CAnimationTreeItem(this, invisibleRootItem());
        addTopLevelItem(pAnimationWidgetItem);

        pAnimationWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pAnimationWidgetItem, pAnimation);

        pAnimationWidgetItem->setName(pAnimation->getName());
        pAnimationWidgetItem->setIcon(0, QIcon(":/Resources/Animation.png"));

        addProperty(pAnimation, pAnimationWidgetItem, DurationKey,		QString::number(pAnimation->getDuration()));
        addProperty(pAnimation, pAnimationWidgetItem, TicksPerSecKey,	QString::number(pAnimation->getTicksPerSecond()));


        foreach (CSceneNodeAnimation* pNA, pAnimation->getNodeAnimations())
        {
            QTreeWidgetItem* pNAWidgetItem = new QTreeWidgetItem();
            pNAWidgetItem->setText(0, pNA->getName());
            pAnimationWidgetItem->addChild(pNAWidgetItem);

            addProperty(pNA, pNAWidgetItem, StateBehaviorKey,	CGeometryGlobal::animationBehaviorTypeEntries(), CGeometryGlobal::stringFromAnimationBehaviorType(pNA->getAnimationBehavior()));


            foreach (const CVectorKey& vK, pNA->positionKeys())
            {
                QTreeWidgetItem* pPKWidgetItem = new QTreeWidgetItem();
                pPKWidgetItem->setText(0, PositionKey);
                pNAWidgetItem->addChild(pPKWidgetItem);

                addProperty(pNA, pPKWidgetItem, TimeKey,	QString::number(vK.getTime()));
                addProperty(pNA, pPKWidgetItem, ValueKey,	vK.getValue());
            }

            foreach (const CQuaternionKey& qK, pNA->rotationKeys())
            {
                QTreeWidgetItem* pRKWidgetItem = new QTreeWidgetItem();
                pRKWidgetItem->setText(0, RotationKey);
                pNAWidgetItem->addChild(pRKWidgetItem);

                addProperty(pNA, pRKWidgetItem, TimeKey,	QString::number(qK.getTime()));
                //addProperty(pNA, pQKWidgetItem, ValueKey,	qK.getValue());
            }

            foreach (const CVectorKey& vK, pNA->scalingKeys())
            {
                QTreeWidgetItem* pSKWidgetItem = new QTreeWidgetItem();
                pSKWidgetItem->setText(0, ScaleKey);
                pNAWidgetItem->addChild(pSKWidgetItem);

                addProperty(pNA, pSKWidgetItem, TimeKey,	QString::number(vK.getTime()));
                addProperty(pNA, pSKWidgetItem, ValueKey,	vK.getValue());
            }
        }

        foreach (CMeshAnimation* pMA, pAnimation->getMeshAnimations())
        {
            QTreeWidgetItem* pMAWidgetItem = new QTreeWidgetItem();
            pMAWidgetItem->setText(0, pMA->getName());
            pAnimationWidgetItem->addChild(pMAWidgetItem);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::createTreeShaders(const QList<CShader*>& shaders)
{
    foreach (CShader* pShader, shaders)
    {
        CEditShaderTreeItem* pSDWidgetItem = new CEditShaderTreeItem(this, invisibleRootItem(), true);
        addTopLevelItem(pSDWidgetItem);
        pSDWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        addEntity(pSDWidgetItem, pShader);

        pSDWidgetItem->setName(pShader->getName());
        pSDWidgetItem->setIcon(0, QIcon(":/Resources/Shaders.png"));

        addFileProperty(pShader, pSDWidgetItem, VertexShaderKey,	pShader->getVertexShaderFileName());
        addFileProperty(pShader, pSDWidgetItem, FragmentShaderKey,	pShader->getFragmentShaderFileName());
        addFileProperty(pShader, pSDWidgetItem, GeometryShaderKey,	pShader->getGeometryShaderFileName());

        foreach (const TUniformValue& uniformValue, pShader->getUniformValues())
        {
            switch (int(uniformValue.second.type()))
            {
            case QMetaType::Int:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, QString::number(uniformValue.second.value<int>()));
                break;
            case QMetaType::Float:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, QString::number(uniformValue.second.value<float>()));
                break;
            case QMetaType::Double:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, QString::number(uniformValue.second.value<double>()));
                break;
            case QMetaType::QVector2D:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, uniformValue.second.value<QVector2D>().toVector3D());
                break;
            case QMetaType::QVector3D:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, uniformValue.second.value<QVector3D>());
                break;
            case QMetaType::QVector4D:
                addProperty(pShader, pSDWidgetItem, uniformValue.first, uniformValue.second.value<QVector4D>());
                break;
            case QMetaType::QMatrix4x4:
                // TODO
                break;
            default:
                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QString& property, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pParent->addChild(pHeaderWidgetItem);

    QTreeWidgetItem* propertyWidgetItem  = new QTreeWidgetItem();
    if (bEditable) propertyWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    propertyWidgetItem->setText(0, property);
    pHeaderWidgetItem->addChild(propertyWidgetItem);

    m_PropertiesItems[propertyWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, bool bProperty, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pHeaderWidgetItem->setCheckState(0, bProperty ? Qt::Checked : Qt::Unchecked);
    if (bEditable)pHeaderWidgetItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    else pHeaderWidgetItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    pParent->addChild(pHeaderWidgetItem);

    m_PropertiesItems[pHeaderWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QList<QString>& properties, const QString& property, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pParent->addChild(pHeaderWidgetItem);

    CComboBoxItem* propertyWidgetItem = new CComboBoxItem(this, pHeaderWidgetItem);

    propertyWidgetItem->addEntries(properties);
    propertyWidgetItem->setCurrentEntry(property);
    propertyWidgetItem->setEnabled(bEditable);

    m_PropertiesItems[propertyWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QVector3D& property, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pParent->addChild(pHeaderWidgetItem);

    CVectorWidgetItem* propertyWidgetItem = new CVectorWidgetItem(this, pHeaderWidgetItem, CVectorWidget::eVectorWidget3D);

    propertyWidgetItem->vectorWidget()->setVector(property);
    propertyWidgetItem->setEnabled(bEditable);

    m_PropertiesItems[propertyWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QVector4D& property, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pParent->addChild(pHeaderWidgetItem);

    CVectorWidgetItem* propertyWidgetItem = new CVectorWidgetItem(this, pHeaderWidgetItem, CVectorWidget::eVectorWidget4D);

    propertyWidgetItem->vectorWidget()->setVector(property);
    propertyWidgetItem->setEnabled(bEditable);

    m_PropertiesItems[propertyWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addFileProperty(AEntity* pEntity, QTreeWidgetItem* pParent, const QString& name, const QString& property, bool bEditable /*= true*/)
{
    QTreeWidgetItem* pHeaderWidgetItem  = new QTreeWidgetItem();
    pHeaderWidgetItem->setText(0, name);
    pParent->addChild(pHeaderWidgetItem);

    CParamShaderItem* propertyWidgetItem = new CParamShaderItem(this, pHeaderWidgetItem, true);

    propertyWidgetItem->setFilePathName(property);
    propertyWidgetItem->setEnabled(bEditable);

    m_PropertiesItems[propertyWidgetItem] = TProperty(pEntity, name);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateProperty(AEntity* pEntity, const QString& name, const QString& property)
{
    QTreeWidgetItem* propertyWidgetItem = m_PropertiesItems.key(TProperty(pEntity, name));

    if (CComboBoxItem* pComboItem = dynamic_cast<CComboBoxItem*>(propertyWidgetItem))
    {
        pComboItem->setCurrentEntry(property);
    }
    else
    {
        propertyWidgetItem->setText(0, property);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateProperty(AEntity* pEntity, const QString& name, bool bProperty)
{
    QTreeWidgetItem* propertyWidgetItem = m_PropertiesItems.key(TProperty(pEntity, name));
    propertyWidgetItem->setCheckState(0, bProperty ? Qt::Checked : Qt::Unchecked);
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateProperty(AEntity* pEntity, const QString& name, const QVector3D& property)
{
    QTreeWidgetItem* propertyWidgetItem = m_PropertiesItems.key(TProperty(pEntity, name));

    if (CVectorWidgetItem* pVWItem = dynamic_cast<CVectorWidgetItem*>(propertyWidgetItem))
    {
        pVWItem->vectorWidget()->setVector(property);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateProperty(AEntity* pEntity, const QString& name, const QVector4D& property)
{
    QTreeWidgetItem* propertyWidgetItem = m_PropertiesItems.key(TProperty(pEntity, name));

    if (CVectorWidgetItem* pVWItem = dynamic_cast<CVectorWidgetItem*>(propertyWidgetItem))
    {
        pVWItem->vectorWidget()->setVector(property);
    }
}

//-----------------------------------------------------------------------------------------
QVector2D CNodeTreeWidget::getVector2DValue(QTreeWidgetItem* pItem)
{
    if (CVectorWidgetItem* pVectorWidgetItem = dynamic_cast<CVectorWidgetItem*>(pItem))
    {
        return pVectorWidgetItem->vectorWidget()->getVector2D();
    }

    qDebug() << "Cet item n'est pas un VectorWidgetItem !";

    return QVector2D();
}

//-----------------------------------------------------------------------------------------
QVector3D CNodeTreeWidget::getVector3DValue(QTreeWidgetItem* pItem)
{
    if (CVectorWidgetItem* pVectorWidgetItem = dynamic_cast<CVectorWidgetItem*>(pItem))
    {
        return pVectorWidgetItem->vectorWidget()->getVector3D();
    }

    qDebug() << "Cet item n'est pas un VectorWidgetItem !";

    return QVector3D();
}

//-----------------------------------------------------------------------------------------
QVector4D CNodeTreeWidget::getVector4DValue(QTreeWidgetItem* pItem)
{
    if (CVectorWidgetItem* pVectorWidgetItem = dynamic_cast<CVectorWidgetItem*>(pItem))
    {
        return pVectorWidgetItem->vectorWidget()->getVector4D();
    }

    qDebug() << "Cet item n'est pas un VectorWidgetItem !";

    return QVector4D();
}

//-----------------------------------------------------------------------------------------
QString CNodeTreeWidget::getComboBoxCurrentEntry(QTreeWidgetItem* pItem)
{
    if (CComboBoxItem* pComboBoxItem = dynamic_cast<CComboBoxItem*>(pItem))
    {
        return pComboBoxItem->getCurrentEntry();
    }

    qDebug() << "Cet item n'est pas un ComboBoxItem !";

    return "";
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::addComboBoxEntry(QTreeWidgetItem* pItem, const QString& entry)
{
    if (CComboBoxItem* pComboBoxItem = dynamic_cast<CComboBoxItem*>(pItem))
    {
        pComboBoxItem->addEntry(entry);
    }

    qDebug() << "Cet item n'est pas un ComboBoxItem !";
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::removeComboBoxEntry(QTreeWidgetItem* pItem, const QString& entry)
{
    if (CComboBoxItem* pComboBoxItem = dynamic_cast<CComboBoxItem*>(pItem))
    {
        pComboBoxItem->removeEntry(entry);
    }

    qDebug() << "Cet item n'est pas un ComboBoxItem !";
}

//-----------------------------------------------------------------------------------------
QString CNodeTreeWidget::getFileSelectorFilePathName(QTreeWidgetItem* pItem)
{
    if (CParamShaderItem* pFileSelectorItem = dynamic_cast<CParamShaderItem*>(pItem))
    {
        return pFileSelectorItem->getFilePathName();
    }

    qDebug() << "Cet item n'est pas un FileSelectorItem !";

    return "";
}

//-----------------------------------------------------------------------------------------
QTreeWidgetItem* CNodeTreeWidget::getPropertyTreeItem(AEntity* pEntity, const QString& name)
{
    return m_PropertiesItems.key(TProperty(pEntity, name));
}

//-----------------------------------------------------------------------------------------
QList<QTreeWidgetItem*> CNodeTreeWidget::getPropertiesTreeItems(AEntity* pEntity)
{
    QList<QTreeWidgetItem*> propertiesTreeItems;

    foreach (const TProperty& property, m_PropertiesItems.values())
    {
        if (property.first == pEntity)
        {
            propertiesTreeItems << m_PropertiesItems.key(property);
        }
    }

    return propertiesTreeItems;
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onItemChanged(QTreeWidgetItem* pItem, int /*iColumnID*/)
{
    if (m_Entities.contains(pItem))
    {
        m_Entities[pItem]->setName(pItem->text(0));
    }

    if (m_PropertiesItems.contains(pItem))
    {
        updateItem(pItem);
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::updateItem(QTreeWidgetItem* pItem)
{
    TProperty property = m_PropertiesItems[pItem];

    AEntity* pEntity = property.first;
    QString propertyName = property.second;

    QString itemText = pItem->text(0);
    bool itemChecked = pItem->checkState(0);

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DU GESTIONNAIRE DE SCENE
    //-----------------------------------------------------------------------------------------

    if (CSceneManager* pSM = dynamic_cast<CSceneManager*>(pEntity))
    {
        // TODO (ambient color)
    }

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DU GESTIONNAIRE DE RENDU
    //-----------------------------------------------------------------------------------------

    if (ARenderer* pRenderer = dynamic_cast<ARenderer*>(pEntity))
    {
        if		(propertyName == RasterizationModeKey)	{ pRenderer->setRasterizationMode(CGeometryGlobal::rasterizationModeFromString(getComboBoxCurrentEntry(pItem))); }
    }

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DES SHADERS
    //-----------------------------------------------------------------------------------------
    else if (CShader* pShader = dynamic_cast<CShader*>(pEntity))
    {
        if		(propertyName == NameKey)				{ pShader->setName(itemText); }
        else if (propertyName == VertexShaderKey)		{ pShader->setVertexShaderFileName(itemText); }
        else if (propertyName == FragmentShaderKey)		{ pShader->setFragmentShaderFileName(getFileSelectorFilePathName(pItem)); }
        else if (propertyName == GeometryShaderKey)		{ pShader->setGeometryShaderFileName(getFileSelectorFilePathName(pItem)); }
        else
        {
            TUniformValue uniformValue = pShader->getUniformValueByName(propertyName);
            if (uniformValue != InvalidUniformValue)
            {
                switch (int(uniformValue.second.type()))
                {
                case QMetaType::Int:
                    pShader->setUniformValue(propertyName, itemText.toInt());
                    break;
                case QMetaType::Float:
                    pShader->setUniformValue(propertyName, itemText.toFloat());
                    break;
                case QMetaType::Double:
                    pShader->setUniformValue(propertyName, itemText.toDouble());
                    break;
                case QMetaType::QVector2D:
                    pShader->setUniformValue(propertyName, getVector2DValue(pItem));
                    break;
                case QMetaType::QVector3D:
                    pShader->setUniformValue(propertyName, getVector3DValue(pItem));
                    break;
                case QMetaType::QVector4D:
                    pShader->setUniformValue(propertyName, getVector4DValue(pItem));
                    break;
                case QMetaType::QMatrix4x4:
                    // TODO
                    break;
                default:
                    break;
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DES ITEMS
    //-----------------------------------------------------------------------------------------

    if (ASceneNodeItem* pNodeItem = dynamic_cast<ASceneNodeItem*>(pEntity))
    {
        if		(propertyName == VisibleKey)	{ pNodeItem->setVisible(itemChecked); }
        else if (propertyName == SelectableKey)	{ pNodeItem->setSelectable(itemChecked); }
        else if (propertyName == SelectedKey)	{ pNodeItem->setSelected(itemChecked); }

        //-----------------------------------------------------------------------------------------
        // TRAITEMENT DES MESHS
        //-----------------------------------------------------------------------------------------

        if (CSubMeshInstance* pSubMeshInstance = dynamic_cast<CSubMeshInstance*>(pNodeItem))
        {
            if (propertyName == MaterialKey) { pSubMeshInstance->setMaterialName(getComboBoxCurrentEntry(pItem)); }
        }

        //-----------------------------------------------------------------------------------------
        // TRAITEMENT DES LUMIERES
        //-----------------------------------------------------------------------------------------

        else if (CLight* pLight = dynamic_cast<CLight*>(pNodeItem))
        {
            if		(propertyName == EnabledKey)				{ pLight->setEnabled(itemChecked); }
            else if	(propertyName == SourceTypeKey)				{ pLight->setSourceType(CGeometryGlobal::lightSourceTypeFromString(getComboBoxCurrentEntry(pItem))); }
            else if	(propertyName == DirectionKey)				{ pLight->setDirection(getVector3DValue(pItem)); }
            else if	(propertyName == ConstantAttenuationKey)	{ pLight->setConstantAttenuation(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == LinearAttenuationKey)		{ pLight->setLinearAttenuation(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == QuadraticAttenuationKey)	{ pLight->setQuadraticAttenuation(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == AmbientColorKey)			{ pLight->setAmbientColor(getVector4DValue(pItem)); }
            else if (propertyName == DiffuseColorKey)			{ pLight->setDiffuseColor(getVector4DValue(pItem)); }
            else if (propertyName == SpecularColorKey)			{ pLight->setSpecularColor(getVector4DValue(pItem)); }
            else if	(propertyName == InnerConeKey)				{ pLight->setInnerConeAngle(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == OuterConeKey)				{ pLight->setOuterConeAngle(CGeometryGlobal::scalaireFromQString(itemText)); }
        }

        //-----------------------------------------------------------------------------------------
        // TRAITEMENT DES CAMERAS
        //-----------------------------------------------------------------------------------------

        else if (CCamera* pCamera = dynamic_cast<CCamera*>(pNodeItem))
        {
            if		(propertyName == EyePositionKey)			{ pCamera->setEyePosition(getVector3DValue(pItem)); }
            else if	(propertyName == CenterKey)					{ pCamera->setCenter(getVector3DValue(pItem)); }
            else if	(propertyName == UpKey)						{ pCamera->setUp(getVector3DValue(pItem)); }
            else if	(propertyName == HorizontalFOVKey)			{ pCamera->setHorizontalFOV(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == ClipPlaneNearKey)			{ pCamera->setNear(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == ClipPlaneFarKey)			{ pCamera->setFar(CGeometryGlobal::scalaireFromQString(itemText)); }
            else if	(propertyName == AspectRatioKey)			{ pCamera->setAspectRatio(CGeometryGlobal::scalaireFromQString(itemText)); }
        }
    }

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DES MATERIAUX
    //-----------------------------------------------------------------------------------------

    else if (CMaterial* pMaterial = dynamic_cast<CMaterial*>(pEntity))
    {
        if		(propertyName == AmbientColorKey)		{ pMaterial->setAmbientColor(getVector4DValue(pItem)); }
        else if (propertyName == DiffuseColorKey)		{ pMaterial->setDiffuseColor(getVector4DValue(pItem)); }
        else if (propertyName == SpecularColorKey)		{ pMaterial->setSpecularColor(getVector4DValue(pItem)); }
        else if (propertyName == AlphaMaskColorKey)		{ pMaterial->setAlphaMaskColor(getVector4DValue(pItem)); }
        else if (propertyName == AlphaMaskFuncKey)		{ pMaterial->setAlphaMaskFunc(CGeometryGlobal::alphaMaskFuncFromString(getComboBoxCurrentEntry(pItem))); }
        else if (propertyName == ShininessFactorKey)	{ pMaterial->setShininessFactor(CGeometryGlobal::scalaireFromQString(itemText)); }
        else if (propertyName == OpacityKey)			{ pMaterial->setOpacity(CGeometryGlobal::scalaireFromQString(itemText)); }
    }

    //-----------------------------------------------------------------------------------------
    // RENDERING PASS
    //-----------------------------------------------------------------------------------------
    else if (CRenderPass* pPass = dynamic_cast<CRenderPass*>(pEntity))
    {
        if (propertyName == ShaderNameKey)
        {
            pPass->setShaderName(itemText);
        }
        else if (propertyName == RenderingTargetKey)
        {
            pPass->setTargetType(CGeometryGlobal::targetTypeFromString(getComboBoxCurrentEntry(pItem)));
        }
        else if (propertyName == ClearColorBufferKey)
        {
            unsigned int flags = pPass->clearBufferFlags();
            itemChecked ? flags |= ColorBuffer : flags & ~ColorBuffer;
            pPass->setClearBufferFlags(flags);
        }
        else if (propertyName == ClearDepthBufferKey)
        {
            unsigned int flags = pPass->clearBufferFlags();
            itemChecked ? flags |= DepthBuffer : flags & ~DepthBuffer;
            pPass->setClearBufferFlags(flags);
        }
        else if (propertyName == ClearStencilBufferKey)
        {
            unsigned int flags = pPass->clearBufferFlags();
            itemChecked ? flags |= StencilBuffer : flags & ~StencilBuffer;
            pPass->setClearBufferFlags(flags);
        }
        else if (propertyName == ClearColorKey) { pPass->setClearColor(getVector4DValue(pItem)); }
    }

    //-----------------------------------------------------------------------------------------
    // DESCRIPTION DE TEXTURES
    //-----------------------------------------------------------------------------------------
    else if (CTexture2D* pTexture = dynamic_cast<CTexture2D*>(pEntity))
    {
        if (propertyName == NameKey)					{ pTexture->setName(itemText); }
        else if (propertyName == FileNamesKey)			{ pTexture->setFileName(itemText); }
    }

    //-----------------------------------------------------------------------------------------
    // TRAITEMENT DES ANIMATIONS
    //-----------------------------------------------------------------------------------------

    else if (CAnimation* pAnimation = dynamic_cast<CAnimation*>(pEntity))
    {
        // TODO
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onUpdate()
{
    updateGeometries();
    //viewport()->update();
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onDeleteClicked()
{
    blockSignals(true);

    if (m_pCurrentEntity)
    {
        m_pCurrentEntity->unregisterListener(this);

        QTreeWidgetItem* pItem = m_Entities.key(m_pCurrentEntity, 0);
        m_Entities.remove(pItem);
        delete pItem;

        if (CMesh* pMesh = dynamic_cast<CMesh*>(m_pCurrentEntity))
        {
            CMeshManager::getInstance().removeMesh(pMesh);
        }
        else if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(m_pCurrentEntity))
        {
            m_pSceneManager->deleteMeshInstance(pMeshInstance);
        }
        else if (CLight* pLight = dynamic_cast<CLight*>(m_pCurrentEntity))
        {
            m_pSceneManager->deleteLight(pLight);
        }
        else if (CCamera* pCamera = dynamic_cast<CCamera*>(m_pCurrentEntity))
        {
            m_pSceneManager->deleteCamera(pCamera);
        }
        else if (CAnimation* pAnimation = dynamic_cast<CAnimation*>(m_pCurrentEntity))
        {
            m_pSceneManager->deleteAnimation(pAnimation);
        }

        m_pCurrentEntity = 0;
    }

    blockSignals(false);

    m_pSceneManager->clearEmptyNodes();
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onSaveClicked()
{
    if (CMesh* pMesh = dynamic_cast<CMesh*>(m_pCurrentEntity))
    {
        QSettings settings("settings.ini");
        QString outDirectory = settings.value("OutDirectory", QCoreApplication::applicationDirPath()).toString();
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Mesh"), outDirectory, tr("*.mesh"));

        if (!fileName.isEmpty())
        {
            settings.setValue("OutDirectory", QFileInfo(fileName).canonicalPath());

            CSaveDialog saveDialog(this);
            saveDialog.move(x() + width() / 2 - saveDialog.width() / 2, y() + height() / 2 - saveDialog.height() / 2);

            if (saveDialog.exec() == QDialog::Accepted)
            {
                CMeshManager::getInstance().saveMesh(pMesh, fileName, 7);
                updateData();
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onSetCurrentClicked()
{
    if (m_pCurrentEntity)
    {
        if (CCamera* pCamera = dynamic_cast<CCamera*>(m_pCurrentEntity))
        {
            m_pView->setCurrentCamera(pCamera);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onUdpateShader(QTreeWidgetItem* pSender)
{
    if (m_PropertiesItems.contains(pSender))
    {
        TProperty property = m_PropertiesItems[pSender];

        AEntity* pEntity = property.first;
        QString propertyName = property.second;

        if (CShader* pShader = dynamic_cast<CShader*>(pEntity))
        {
            if		(propertyName == VertexShaderKey)		{ pShader->setVertexShaderCode(m_pEditItem->getText()); }
            else if (propertyName == FragmentShaderKey)		{ pShader->setFragmentShaderCode(m_pEditItem->getText()); }
            else if (propertyName == GeometryShaderKey)		{ pShader->setGeometryShaderCode(m_pEditItem->getText()); }
        }
    }
}

//-----------------------------------------------------------------------------------------
void CNodeTreeWidget::onSaveShader(QTreeWidgetItem* pSender)
{
    if (m_PropertiesItems.contains(pSender))
    {
        TProperty property = m_PropertiesItems[pSender];

        AEntity* pEntity = property.first;
        QString propertyName = property.second;

        if (CShader* pShader = dynamic_cast<CShader*>(pEntity))
        {
            if		(propertyName == VertexShaderKey)		{ pShader->saveVertexShader(); }
            else if (propertyName == FragmentShaderKey)		{ pShader->saveFragmentShader(); }
            else if (propertyName == GeometryShaderKey)		{ pShader->saveGeometryShader(); }
        }
    }
}

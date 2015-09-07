#include "CScriptManager.h"

// Geometry
#include "CMaterialManager.h"
#include "CShaderManager.h"

// Assimp
#include "CAssimpImporter.h"

//-----------------------------------------------------------------------------------------
CScriptManager::CScriptManager(CSceneManager* pSceneManager)
: m_pSceneManager(pSceneManager)
, m_bSaveOnlyUsed(true)
, m_bSaveData(true)
{
}

//-----------------------------------------------------------------------------------------
bool CScriptManager::loadScript(const QString& fileName, CSceneNode* pNode)
{
    CXmlReader xmlReader(false);

    if (xmlReader.loadFile(fileName))
    {
        m_CurrentPath = QFileInfo(fileName).absolutePath() + "/";

        CXmlElement* pRootNode = xmlReader.getRootElement();

        recursiveLoad(pRootNode, pNode);
        //m_pSceneManager->getRootNode()->dumpNodeTree();

        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void CScriptManager::recursiveLoad(CXmlElement* pElement, CSceneNode* pCurrentNode)
{
    foreach (CXmlElement* pChild, pElement->getChildElements())
    {
        if (pChild->getName() == "SceneNode")
        {
            QString name = pChild->getAttributeValue<QString>("name");
            QVector3D translation = pChild->getAttributeValue<QVector3D>("translation");
            QQuaternion rotation = QQuaternion(pChild->getAttributeValue<QVector4D>("rotation"));

            recursiveLoad(pChild, pCurrentNode->createChild(name, translation, rotation));
        }
        else if (pChild->getName() == "Shader")
        {
            QString name = pChild->getAttributeValue<QString>("name");
            QString vertexFileName = pChild->getAttributeValue<QString>("vertexShader");
            QString fragmentFileName = pChild->getAttributeValue<QString>("fragmentShader");
            QString geometryFileName = pChild->getAttributeValue<QString>("geometryShader");

            /*CShader* pShader = */ CShaderManager::getInstance()->createShader(
                        name,
                        path(vertexFileName),
                        path(geometryFileName),
                        path(fragmentFileName));

            // TODO : éventuellement setUniformValue
        }
        else if (pChild->getName() == "Texture")
        {
            QString name = pChild->getAttributeValue<QString>("name");
            EnumTextureType eType = CGeometryGlobal::textureTypeFromString(pChild->getAttributeValue<QString>("type"));

            switch (eType)
            {
                case eTexture2D:
                {
                    QString fileName = pChild->getAttributeValue<QString>("fileName");
                    if (!fileName.isEmpty())
                    {
                        CTextureManager::getInstance()->createTexture2D(name, path(fileName));
                    }
                    else
                    {
                        QVector2D size = pChild->getAttributeValue<QVector2D>("size");
                        if (!size.isNull())
                        {
                            CTextureManager::getInstance()->createTexture2D(name, QSize(size.x(), size.y()));
                        }
                    }
                }
                break;

                case eTextureCube:
                {
                    QString fileNamePosX = pChild->getAttributeValue<QString>("fileNamePosX");
                    QString fileNameNegX = pChild->getAttributeValue<QString>("fileNameNegX");
                    QString fileNamePosY = pChild->getAttributeValue<QString>("fileNamePosY");
                    QString fileNameNegY = pChild->getAttributeValue<QString>("fileNameNegY");
                    QString fileNamePosZ = pChild->getAttributeValue<QString>("fileNamePosZ");
                    QString fileNameNegZ = pChild->getAttributeValue<QString>("fileNameNegZ");

                    CTextureManager::getInstance()->createTextureCube(
                                name,
                                path(fileNamePosX),
                                path(fileNameNegX),
                                path(fileNamePosY),
                                path(fileNameNegY),
                                path(fileNamePosZ),
                                path(fileNameNegZ));
                }
                break;

                case eTextureTarget:
                {
                    QVector2D size = pChild->getAttributeValue<QVector2D>("size");
                    EnumInternalFormat eFormat = CGeometryGlobal::internalFormatFromString(pChild->getAttributeValue<QString>("internalFormat"));

                    CTextureManager::getInstance()->createTextureTarget(
                                name,
                                QSize(size.x(), size.y()),
                                eFormat);
                }
                break;

                case eTexture3D:
                {
                    // TODO
                }
                break;

                case eTextureUndefined:
                    break;
            }
        }
        else if (pChild->getName() == "Material")
        {
            QString name = pChild->getAttributeValue<QString>("name");

            CMaterial* pMaterial = CMaterialManager::getInstance()->createMaterial(name);

            if (CXmlElement* pRenderPassesElement = pChild->getChildByName("RenderPasses"))
            {
                foreach (CXmlElement* pRenderPassElement, pRenderPassesElement->getChildElementsByName("RenderPass"))
                {
                    EnumTarget eTarget = CGeometryGlobal::targetTypeFromString(pRenderPassElement->getAttributeValue<QString>("target"));
                    QVector2D size = pRenderPassElement->getAttributeValue<QVector2D>("size");

                    CRenderStates states;
                    if (CXmlElement* pRenderStatesElement = pChild->getChildByName("RenderStates"))
                    {
                        // TODO
                        /*
                        CFaceCulling m_FaceCulling;
                        CScissorTest m_ScissorTest;
                        CDepthTest m_DepthTest;
                        CDepthRange m_DepthRange;
                        CBlending m_Blending;
                        CColorMask m_ColorMask;
                        CStencilState m_StencilState;*/
                    }

                    pMaterial->createRenderPass(eTarget, QSize(size.x(), size.y()), states);
                }
            }

            if (CXmlElement* pTexturesElement = pChild->getChildByName("Textures"))
            {
                foreach (CXmlElement* pTextureElement, pTexturesElement->getChildElementsByName("Texture"))
                {
                    QString textureName = pTextureElement->getAttributeValue<QString>("name");
                    EnumMaterialParameter eMatParam = CGeometryGlobal::materialParameterFromString(pTextureElement->getAttributeValue<QString>("materialParameter"));
                    pMaterial->addTexture(textureName, eMatParam);
                }
            }

            pMaterial->setAmbientColor(pChild->getAttributeValue<QVector4D>("ambientColor", DefaultMaterialAmbientColor));
            pMaterial->setDiffuseColor(pChild->getAttributeValue<QVector4D>("diffuseColor", DefaultMaterialDiffuseColor));
            pMaterial->setSpecularColor(pChild->getAttributeValue<QVector4D>("specularColor", DefaultMaterialSpecularColor));
            pMaterial->setAlphaMaskColor(pChild->getAttributeValue<QVector4D>("alphaMaskColor", DefaultMaterialAlphaMaskColor));
            pMaterial->setAlphaMaskFunc(CGeometryGlobal::alphaMaskFuncFromString(pChild->getAttributeValue<QString>("alphaMaskFunc")));
            pMaterial->setShininessFactor(pChild->getAttributeValue<real>("shininessFactor", DefaultMaterialShininessFactor));
            pMaterial->setOpacity(pChild->getAttributeValue<real>("opacity", 1.0));
        }
        else if (pChild->getName() == "Scene")
        {
            QString fileName = pChild->getAttributeValue<QString>("fileName");
            bool bMergeMaterial = pChild->getAttributeValue<bool>("mergeMaterial", true);
			bool bJustLoad = pChild->getAttributeValue<bool>("justLoad", false);

			if (bJustLoad)
			{
				CAssimpImporter::loadScene(
                    path(fileName),
					m_pSceneManager, 
					bMergeMaterial);
			}
			else
			{
				CAssimpImporter::mergeScene(
                    path(fileName),
					m_pSceneManager, 
					bMergeMaterial, 
					pCurrentNode);
			}
        }
        else if (pChild->getName() == "CreateMesh")
        {

            QString name = pChild->getAttributeValue<QString>("name");
            //QString typeName = pChild->getAttributeValue<QString>("typeName");
            //CMeshManager::getInstance()->createCustomMesh(type, name);

        }
        else if (pChild->getName() == "MeshGroup")
        {
            QString fileName = pChild->getAttributeValue<QString>("fileName");
            CMeshManager::getInstance()->loadMeshGroup(path(fileName));
        }
        else // SceneManager contents
        {
            AEntity* pEntity = 0;

            if (pChild->getName() == "MeshInstance")
            {
                QString meshName = pChild->getAttributeValue<QString>("meshName");

                CMesh* pMesh = 0;
                if (!m_MeshNames.contains(meshName))
                {
                    pMesh = CMeshManager::getInstance()->loadMesh(path(meshName + ".mesh"));
                    m_MeshNames[meshName] = pMesh->getName();
                }
                else
                {
                    pMesh = CMeshManager::getInstance()->getMeshByName(m_MeshNames[meshName]);
                }

                QString name = pChild->getAttributeValue<QString>("name");
                CMeshInstance* pMeshInstance = m_pSceneManager->createMeshInstance(pMesh, name);
                QString materialName = pChild->getAttributeValue<QString>("materialName");
                if (!materialName.isEmpty())
                {
                    pMeshInstance->setMaterialName(materialName);
                }
                pEntity = pMeshInstance;
            }
            else if (pChild->getName() == "Light")
            {
                QString name = pChild->getAttributeValue<QString>("name");
                CLight* pLight = m_pSceneManager->createLight(name);
                pLight->setSourceType(CGeometryGlobal::lightSourceTypeFromString(pChild->getAttributeValue<QString>("sourceType")));
                pLight->setDirection(pChild->getAttributeValue<QVector3D>("direction"));
                pLight->setConstantAttenuation(pChild->getAttributeValue<real>("constantAttenuation"));
                pLight->setLinearAttenuation(pChild->getAttributeValue<real>("linearAttenuation"));
                pLight->setQuadraticAttenuation(pChild->getAttributeValue<real>("quadraticAttenuation"));
                pLight->setAmbientColor(pChild->getAttributeValue<QVector4D>("ambientColor"));
                pLight->setDiffuseColor(pChild->getAttributeValue<QVector4D>("diffuseColor"));
                pLight->setSpecularColor(pChild->getAttributeValue<QVector4D>("specularColor"));
                pLight->setInnerConeAngle(pChild->getAttributeValue<real>("innerConeAngle"));
                pLight->setOuterConeAngle(pChild->getAttributeValue<real>("outerConeAngle"));
                pEntity = pLight;
            }
            else if (pChild->getName() == "Camera")
            {
                QString name = pChild->getAttributeValue<QString>("name");
                CCamera* pCamera = m_pSceneManager->createCamera(name);
                pCamera->setProjectionType(CGeometryGlobal::projectionTypeFromString(pChild->getAttributeValue<QString>("projectionType")));
                pCamera->setEyePosition(pChild->getAttributeValue<QVector3D>("eyePosition"));
                pCamera->setUp(pChild->getAttributeValue<QVector3D>("up"));
                pCamera->setCenter(pChild->getAttributeValue<QVector3D>("center"));
                pCamera->setHorizontalFOV(pChild->getAttributeValue<real>("horizontalFOV"));
                pCamera->setNear(pChild->getAttributeValue<real>("clipPlaneNear"));
                pCamera->setFar(pChild->getAttributeValue<real>("clipPlaneFar"));
                pCamera->setAspectRatio(pChild->getAttributeValue<real>("aspectRatio"));
                pEntity = pCamera;
            }
            else if (pChild->getName() == "Animation")
            {
                QString name = pChild->getAttributeValue<QString>("name");
                CAnimation* pAnimation = m_pSceneManager->createAnimation(name);

                if (CXmlElement* pNodeAnimationsElement = pChild->getChildByName("NodeAnimations"))
                {
                    foreach (CXmlElement* pNodeAnimationElement, pNodeAnimationsElement->getChildElementsByName("NodeAnimation"))
                    {
                        QString nodeName = pNodeAnimationElement->getAttributeValue<QString>("nodeName");
                        CSceneNodeAnimation* pNodeAnimation = pAnimation->createNodeAnimation(nodeName);
                        EnumAnimationBehaviour eBehavior = CGeometryGlobal::animationBehaviorTypeFromString(pNodeAnimationElement->getAttributeValue<QString>("behavior"));
                        pNodeAnimation->setAnimationBehavior(eBehavior);

                        if (CXmlElement* pPosKeysElement = pNodeAnimationElement->getChildByName("PosKeys"))
                        {
                            foreach (CXmlElement* pPosKey, pPosKeysElement->getChildElementsByName("PosKey"))
                            {
                                QVector3D value = pPosKey->getAttributeValue<QVector3D>("value");
                                double dTime = pPosKey->getAttributeValue<double>("time");
                                pNodeAnimation->positionKeys() << CVectorKey(dTime, value);
                            }
                        }

                        if (CXmlElement* pRotKeysElement = pNodeAnimationElement->getChildByName("RotKeys"))
                        {
                            foreach (CXmlElement* pRotKey, pRotKeysElement->getChildElementsByName("RotKey"))
                            {
                                QQuaternion value = QQuaternion(pRotKey->getAttributeValue<QVector4D>("value"));
                                double dTime = pRotKey->getAttributeValue<double>("time");
                                pNodeAnimation->rotationKeys() << CQuaternionKey(dTime, value);
                            }
                        }

                        if (CXmlElement* pScaKeysElement = pNodeAnimationElement->getChildByName("ScaKeys"))
                        {
                            foreach (CXmlElement* pScaKey, pScaKeysElement->getChildElementsByName("ScaKey"))
                            {
                                QVector3D value = pScaKey->getAttributeValue<QVector3D>("value");
                                double dTime = pScaKey->getAttributeValue<double>("time");
                                pNodeAnimation->scalingKeys() << CVectorKey(dTime, value);
                            }
                        }
                    }
                }

                if (CXmlElement* pMeshAnimationsElement = pChild->getChildByName("MeshAnimations"))
                {
                    foreach (CXmlElement* pMeshAnimationElement, pMeshAnimationsElement->getChildElementsByName("MeshAnimation"))
                    {
                        QString meshName = pMeshAnimationElement->getAttributeValue<QString>("meshName");
                        CMeshAnimation* pMeshAnimation = pAnimation->createMeshAnimation(meshName);

                        if (CXmlElement* pMeshKeysElement = pMeshAnimationElement->getChildByName("MeshKeys"))
                        {
                            foreach (CXmlElement* pMeshKey, pMeshKeysElement->getChildElementsByName("MeshKey"))
                            {
                                unsigned int value = pMeshKey->getAttributeValue<unsigned int>("value");
                                double dTime = pMeshKey->getAttributeValue<double>("time");
                                pMeshAnimation->keys() << CMeshKey(dTime, value);
                            }
                        }
                    }
                }

                pAnimation->setDuration(pChild->getAttributeValue<real>("duration", -1));
                pAnimation->setTicksPerSecond(pChild->getAttributeValue<real>("ticksPerSecond", DefaultTicksPerSecond));
                pAnimation->start();

                pEntity = pAnimation;
            }

            if (pEntity)
            {
                if (ASceneNodeItem* pSceneNodeItem = dynamic_cast<ASceneNodeItem*>(pEntity))
                {
                    pCurrentNode->addItem(pSceneNodeItem);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
bool CScriptManager::saveScript(const QString& fileName)
{
    m_CurrentPath = QFileInfo(fileName).absolutePath() + "/";

    if (m_bSaveData) saveData();
    CXmlElement rootElement("root");

    QSet<CShader*> shaders = m_bSaveOnlyUsed ? m_pSceneManager->getUsedShaders() : CShaderManager::getInstance()->getShaders().toSet();
    foreach (CShader* pShader, shaders)
    {
        CXmlElement* pChild = rootElement.createChild("Shader");
        pChild->addAttribute("name", pShader->getName());
        pChild->addAttribute("vertexShader", QFileInfo(pShader->getVertexShaderFileName()).fileName());
        pChild->addAttribute("fragmentShader", QFileInfo(pShader->getFragmentShaderFileName()).fileName());
        pChild->addAttribute("geometryShader", QFileInfo(pShader->getGeometryShaderFileName()).fileName());
    }

    QSet<ATexture*> textures = m_bSaveOnlyUsed ? m_pSceneManager->getUsedTextures() : CTextureManager::getInstance()->getTextures().toSet();
    foreach (ATexture* pTexture, textures)
    {
        CXmlElement* pChild = rootElement.createChild("Texture");
        pChild->addAttribute("name", pTexture->getName());
        pChild->addAttribute("type", CGeometryGlobal::stringFromTextureType(pTexture->getType()));

        if (CTexture2D* pTexture2D = dynamic_cast<CTexture2D*>(pTexture))
        {
            pChild->addAttribute("fileName", QFileInfo(pTexture2D->getFileName()).fileName());
            pChild->addAttribute("size", QVector2D(pTexture2D->getSize().width(), pTexture2D->getSize().height()));
        }
        else if (CTextureCube* pTextureCube = dynamic_cast<CTextureCube*>(pTexture))
        {
            pChild->addAttribute("fileNamePosX", QFileInfo(pTextureCube->getFileNames()[0]).fileName());
            pChild->addAttribute("fileNameNegX", QFileInfo(pTextureCube->getFileNames()[1]).fileName());
            pChild->addAttribute("fileNamePosY", QFileInfo(pTextureCube->getFileNames()[2]).fileName());
            pChild->addAttribute("fileNameNegY", QFileInfo(pTextureCube->getFileNames()[3]).fileName());
            pChild->addAttribute("fileNamePosZ", QFileInfo(pTextureCube->getFileNames()[4]).fileName());
            pChild->addAttribute("fileNameNegZ", QFileInfo(pTextureCube->getFileNames()[5]).fileName());
        }
        else if (CTextureTarget* pTextureTarget = dynamic_cast<CTextureTarget*>(pTexture))
        {
            pChild->addAttribute("size", QVector2D(pTexture2D->getSize().width(), pTexture2D->getSize().height()));
            pChild->addAttribute("internalFormat", CGeometryGlobal::stringFromInternalFormat(pTextureTarget->getFormat()));
        }
        else if (CTexture3D* pTexture3D = dynamic_cast<CTexture3D*>(pTexture))
        {
            // TODO
        }
    }

    QSet<CMaterial*> materials = m_bSaveOnlyUsed ? m_pSceneManager->getUsedMaterials() : CMaterialManager::getInstance()->getMaterials().toSet();
    foreach (CMaterial* pMaterial, materials)
    {
        CXmlElement* pChild = rootElement.createChild("Material");
        pChild->addAttribute("name", pMaterial->getName());

        CXmlElement* pRenderPassesElement = pChild->createChild("RenderPasses");
        foreach (CRenderPass* pRenderPass, pMaterial->getRenderPasses())
        {
            CXmlElement* pRenderPassElement = pRenderPassesElement->createChild("RenderPass");

            pRenderPassElement->addAttribute("target", CGeometryGlobal::stringFromTargetType(pRenderPass->getTargetType()));
            pRenderPassElement->addAttribute("size", QVector2D(pRenderPass->getSize().width(), pRenderPass->getSize().height()));
            pRenderPassElement->addAttribute("shader", pRenderPass->getShaderName());
            pRenderPassElement->addAttribute("camera", pRenderPass->getCamera()->getName());
            pRenderPassElement->addAttribute("clearBufferFlags", pRenderPass->getClearBufferFlags());

            const CRenderStates& renderStates = pRenderPass->getRenderStates();
            // TODO

//            CXmlElement* pRenderStatesElement = pRenderPassElement->createChild("RenderStates");
//            CXmlElement* pFaceCullingElement = pRenderStatesElement->createChild("FaceCulling");
//            pFaceCullingElement->addAttribute("enabled", renderStates.getFaceCulling().isEnabled());
//            pFaceCullingElement->addAttribute("cullFace", renderStates.getFaceCulling().isEnabled());

//            CXmlElement* pScissorTestElement = pRenderStatesElement->createChild("ScissorTest");
//            pScissorTestElement->addAttribute("enabled", renderStates.getScissorTest().isEnabled());
//            pScissorTestElement->addAttribute("rect", renderStates.getScissorTest().getRect());

//            CXmlElement* pDepthTestElement = pRenderStatesElement->createChild("DepthTest");
//            pDepthTestElement->addAttribute("enabled", renderStates.getDepthTest().isEnabled());
//            pDepthTestElement->addAttribute("function", renderStates.getDepthTest().getFunction());

//            CXmlElement* pDepthRangeElement = pRenderStatesElement->createChild("DepthRange");
//            pDepthRangeElement->addAttribute("near", renderStates.getDepthRange().getNear());
//            pDepthRangeElement->addAttribute("far", renderStates.getDepthRange().getFar());

//            CXmlElement* pBlendingElement = pRenderStatesElement->createChild("Blending");

//            pBlendingElement->addAttribute("srcRGBFactor", renderStates.getBlending().getSourceRGBFactor());
//            pBlendingElement->addAttribute("srcAlphaFactor", renderStates.getBlending().getSourceAlphaFactor());
//            pBlendingElement->addAttribute("destRGBFactor", renderStates.getBlending().getDestinationRGBFactor());
//            pBlendingElement->addAttribute("destAlphaFactor", renderStates.getBlending().getDestinationAlphaFactor());
//            pBlendingElement->addAttribute("rgbEquation", renderStates.getBlending().getRGBEquation());
//            pBlendingElement->addAttribute("alphaEquation", renderStates.getBlending().getAlphaEquation());
//            pBlendingElement->addAttribute("color", renderStates.getBlending().getColor());

//            CXmlElement* pColorMaskElement = pRenderStatesElement->createChild("ColorMask");
//            pColorMaskElement->addAttribute("isRedEnabled", renderStates.getColorMask().isRedEnabled());
//            pColorMaskElement->addAttribute("isGreenEnabled", renderStates.getColorMask().isGreenEnabled());
//            pColorMaskElement->addAttribute("isBlueEnabled", renderStates.getColorMask().isBlueEnabled());
//            pColorMaskElement->addAttribute("isAlphaEnabled", renderStates.getColorMask().isAlphaEnabled());

//            CXmlElement* pStencilStateElement = pRenderStatesElement->createChild("StencilState");
//            pStencilStateElement->addAttribute("enabled", renderStates.getStencilState().isEnabled());
//            pStencilStateElement->addAttribute("frontFace", renderStates.getStencilState().getFrontFace());
//            pStencilStateElement->addAttribute("backFace", renderStates.getStencilState().getBackFace());
//            pStencilStateElement->addAttribute("function", renderStates.getStencilState().getStencilTestFunction());
//            pStencilStateElement->addAttribute("referenceValue", renderStates.getStencilState().getReferenceValue());
//            pStencilStateElement->addAttribute("mask", renderStates.getStencilState().getMask());
        }

        CXmlElement* pTexturesElement = pChild->createChild("Textures");
        foreach (const CTextureParam& textureParam, pMaterial->getTextureParams())
        {
            CXmlElement* pTextureElement = pTexturesElement->createChild("Texture");
            pTextureElement->addAttribute("name", textureParam.getTextureName());
            pTextureElement->addAttribute("materialParameter", CGeometryGlobal::stringFromMaterialParameter(textureParam.getMaterialParameter()));
        }

        pChild->addAttribute("ambientColor", pMaterial->getAmbientColor());
        pChild->addAttribute("diffuseColor", pMaterial->getDiffuseColor());
        pChild->addAttribute("specularColor", pMaterial->getSpecularColor());
        pChild->addAttribute("alphaMaskColor", pMaterial->getAlphaMaskColor());
        pChild->addAttribute("alphaMaskFunc", CGeometryGlobal::stringFromAlphaMaskFunc(pMaterial->getAlphaMaskFunc()));
        pChild->addAttribute("shininessFactor", pMaterial->getShininessFactor());
        pChild->addAttribute("opacity", pMaterial->getOpacity());
    }

    foreach (CAnimation* pAnimation, m_pSceneManager->getAnimations())
    {
        CXmlElement* pChild = rootElement.createChild("Animation");
        pChild->addAttribute("name", pAnimation->getName());

        CXmlElement* pNodeAnimationsElement = pChild->createChild("NodeAnimations");
        foreach (CSceneNodeAnimation* pNodeAnimation, pAnimation->getNodeAnimations())
        {
            CXmlElement* pNodeAnimationElement = pNodeAnimationsElement->createChild("NodeAnimation");

            pNodeAnimationElement->addAttribute("nodeName", pNodeAnimation->getNodeName());
            pNodeAnimationElement->addAttribute("behavior", CGeometryGlobal::stringFromAnimationBehaviorType(pNodeAnimation->getAnimationBehavior()));

            CXmlElement* pPosKeysElement = pNodeAnimationElement->createChild("PosKeys");
            foreach (const CVectorKey& posKey, pNodeAnimation->positionKeys())
            {
                CXmlElement* pPosKeyElement = pPosKeysElement->createChild("PosKey");
                pPosKeyElement->addAttribute("value", posKey.getValue());
                pPosKeyElement->addAttribute("time", posKey.getTime());
            }

            CXmlElement* pRotKeysElement = pNodeAnimationElement->createChild("RotKeys");
            foreach (const CQuaternionKey& rotKey, pNodeAnimation->rotationKeys())
            {
                CXmlElement* pRotKeyElement = pRotKeysElement->createChild("RotKey");
                pRotKeyElement->addAttribute("value", rotKey.getValue());
                pRotKeyElement->addAttribute("time", rotKey.getTime());
            }

            CXmlElement* pScaKeysElement = pNodeAnimationElement->createChild("ScaKeys");
            foreach (const CVectorKey& scaKey, pNodeAnimation->scalingKeys())
            {
                CXmlElement* pScaKeyElement = pScaKeysElement->createChild("ScaKey");
                pScaKeyElement->addAttribute("value", scaKey.getValue());
                pScaKeyElement->addAttribute("time", scaKey.getTime());
            }
        }

        CXmlElement* pMeshAnimationsElement = pChild->createChild("MeshAnimations");
        foreach (CMeshAnimation* pMeshAnimation, pAnimation->getMeshAnimations())
        {
            CXmlElement* pMeshAnimationElement = pMeshAnimationsElement->createChild("MeshAnimation");
            pMeshAnimationElement->addAttribute("meshName", pMeshAnimation->getMeshInstanceName());

            CXmlElement* pMeshKeysElement = pMeshAnimationElement->createChild("MeshKeys");
            foreach (const CMeshKey& meshKey, pMeshAnimation->keys())
            {
                CXmlElement* pMeshKeyElement = pMeshKeysElement->createChild("MeshKey");
                pMeshKeyElement->addAttribute("value", meshKey.getValue());
                pMeshKeyElement->addAttribute("time", meshKey.getTime());
            }
        }

        pChild->addAttribute("duration", pAnimation->getDuration());
        pChild->addAttribute("ticksPerSecond", pAnimation->getTicksPerSecond());
    }

    // Root node
    CXmlElement* pRootXmlElement = rootElement.createChild("SceneNode");
    pRootXmlElement->addAttribute("name", m_pSceneManager->getRootNode()->getName());
    pRootXmlElement->addAttribute("translation", m_pSceneManager->getRootNode()->getGlobalPosition());

    // Caméras libres - On les attaches au noeud principal
    foreach (CCamera* pCamera, m_pSceneManager->getCameras())
    {
        if (pCamera->getNodes().isEmpty())
        {
            CXmlElement* pChild = rootElement.createChild("Camera");
            pChild->addAttribute("projectionType", CGeometryGlobal::stringFromProjectionType(pCamera->getProjectionType()));
            pChild->addAttribute("eyePosition", pCamera->getEyePosition());
            pChild->addAttribute("up", pCamera->getUp());
            pChild->addAttribute("center", pCamera->getCenter());
            pChild->addAttribute("horizontalFOV", pCamera->getHorizontalFOV());
            pChild->addAttribute("clipPlaneNear", pCamera->getNear());
            pChild->addAttribute("clipPlaneFar", pCamera->getFar());
            pChild->addAttribute("aspectRatio", pCamera->getAspectRatio());
        }
    }


    recursiveSave(pRootXmlElement, m_pSceneManager->getRootNode());

    return CXmlWriter::saveFile(fileName, &rootElement);
}

void CScriptManager::recursiveSave(CXmlElement* pElement, CSceneNode* pCurrentNode)
{
    foreach (ASceneNodeItem* pItem, pCurrentNode->getItems())
    {
        pElement->addAttribute("name", pItem->getName());
        if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(pItem))
        {
            CXmlElement* pChild = pElement->createChild("MeshInstance");
            pChild->addAttribute("meshName", pMeshInstance->getMesh()->getName());
            pChild->addAttribute("materialName", pMeshInstance->getMaterialName());
        }
        else if (CLight* pLight = dynamic_cast<CLight*>(pItem))
        {
            CXmlElement* pChild = pElement->createChild("Light");
            pChild->addAttribute("sourceType", CGeometryGlobal::stringFromLightSourceType(pLight->getSourceType()));
            pChild->addAttribute("direction", pLight->getDirection());
            pChild->addAttribute("constantAttenuation", pLight->getConstantAttenuation());
            pChild->addAttribute("linearAttenuation", pLight->getLinearAttenuation());
            pChild->addAttribute("quadraticAttenuation", pLight->getQuadraticAttenuation());
            pChild->addAttribute("ambientColor", pLight->getAmbientColor());
            pChild->addAttribute("diffuseColor", pLight->getDiffuseColor());
            pChild->addAttribute("specularColor", pLight->getSpecularColor());
            pChild->addAttribute("innerConeAngle", pLight->getInnerConeAngle());
            pChild->addAttribute("outerConeAngle", pLight->getOuterConeAngle());
        }
        else if (CCamera* pCamera = dynamic_cast<CCamera*>(pItem))
        {
            CXmlElement* pChild = pElement->createChild("Camera");
            pChild->addAttribute("projectionType", CGeometryGlobal::stringFromProjectionType(pCamera->getProjectionType()));
            pChild->addAttribute("eyePosition", pCamera->getEyePosition());
            pChild->addAttribute("up", pCamera->getUp());
            pChild->addAttribute("center", pCamera->getCenter());
            pChild->addAttribute("horizontalFOV", pCamera->getHorizontalFOV());
            pChild->addAttribute("clipPlaneNear", pCamera->getNear());
            pChild->addAttribute("clipPlaneFar", pCamera->getFar());
            pChild->addAttribute("aspectRatio", pCamera->getAspectRatio());
        }
    }

    foreach (CSceneNode* pChildNode, pCurrentNode->getChildNodes())
    {
        CXmlElement* pXmlElement = pElement->createChild("SceneNode");
        pXmlElement->addAttribute("name", pChildNode->getName());
        pXmlElement->addAttribute("translation", pChildNode->getGlobalPosition());
        recursiveSave(pXmlElement, pChildNode);
    }
}

void CScriptManager::saveData()
{
    foreach (CMesh* pMesh, CMeshManager::getInstance()->getMeshs())
    {
        CMeshManager::getInstance()->saveMesh(pMesh, m_CurrentPath + pMesh->getName() + ".mesh", 7);
    }

    QSet<ATexture*> textures = m_bSaveOnlyUsed ? m_pSceneManager->getUsedTextures() : CTextureManager::getInstance()->getTextures().toSet();
    foreach (ATexture* pTexture, textures)
    {
        if (CTexture2D* pTexture2D = dynamic_cast<CTexture2D*>(pTexture))
        {
            QFile::copy(pTexture2D->getFileName(), m_CurrentPath + QFileInfo(pTexture2D->getFileName()).fileName());
        }
        else if (CTextureCube* pTextureCube = dynamic_cast<CTextureCube*>(pTexture))
        {
            foreach (const QString& fileName, pTextureCube->getFileNames())
            {
                QFile::copy(fileName, m_CurrentPath + QFileInfo(fileName).fileName());
            }
        }
    }

    QSet<CShader*> shaders = m_bSaveOnlyUsed ? m_pSceneManager->getUsedShaders() : CShaderManager::getInstance()->getShaders().toSet();
    foreach (CShader* pShader, shaders)
    {
        if (pShader->hasVertexShader())
        {
            QFile::copy(pShader->getVertexShaderFileName(), m_CurrentPath + QFileInfo(pShader->getVertexShaderFileName()).fileName());
        }
        if (pShader->hasFragmentShader())
        {
            QFile::copy(pShader->getFragmentShaderFileName(), m_CurrentPath + QFileInfo(pShader->getFragmentShaderFileName()).fileName());
        }
        if (pShader->hasGeometryShader())
        {
            QFile::copy(pShader->getGeometryShaderFileName(), m_CurrentPath + QFileInfo(pShader->getGeometryShaderFileName()).fileName());
        }
        if (pShader->hasComputeShader())
        {
            QFile::copy(pShader->getComputeShaderFileName(), m_CurrentPath + QFileInfo(pShader->getComputeShaderFileName()).fileName());
        }
        if (pShader->hasTessellationControlShader())
        {
            QFile::copy(pShader->getTessellationControlShaderFileName(), m_CurrentPath + QFileInfo(pShader->getTessellationControlShaderFileName()).fileName());
        }
        if (pShader->hasTessellationEvaluationShader())
        {
            QFile::copy(pShader->getTessellationEvaluationShaderFileName(), m_CurrentPath + QFileInfo(pShader->getTessellationEvaluationShaderFileName()).fileName());
        }
    }
}

QString CScriptManager::path(const QString& fileName)
{
    if (!fileName.isEmpty())
    {
        return QFileInfo(fileName).isAbsolute() ? fileName : m_CurrentPath + fileName;
    }
    return QString();
}

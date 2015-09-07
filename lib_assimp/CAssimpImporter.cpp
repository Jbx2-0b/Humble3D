#include "CAssimpImporter.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"

// Qt
#include <QFileInfo>


//-----------------------------------------------------------------------------------------
void CAssimpMesh::setAiMesh(aiMesh* pAssimpMesh)
{
    m_pAssimpMesh = pAssimpMesh;
    update();
}

//-----------------------------------------------------------------------------------------
void CAssimpMesh::updateGeometry()
{
    if (m_pAssimpMesh)
    {
        CSubMesh* pBuffer = createSubMesh();

        if (m_pAssimpMesh->HasPositions())
        {
            CBuffer<QVector3D>& vertexPositionBuffer = pBuffer->positionsBuffer();
            CBuffer<QVector3D>& vertexNormalBuffer = pBuffer->normalsBuffer();
            CBuffer<QVector3D>& vertexTangentBuffer = pBuffer->tangentsBuffer();
            CBuffer<QVector3D>& vertexBitangentBuffer = pBuffer->bitangentsBuffer();

            for (unsigned int iVertex = 0; iVertex < m_pAssimpMesh->mNumVertices; ++iVertex)
            {
                vertexPositionBuffer << CAssimpImporter::aiVector3DToQVector3D(m_pAssimpMesh->mVertices[iVertex]);
                vertexNormalBuffer << CAssimpImporter::aiVector3DToQVector3D(m_pAssimpMesh->mNormals[iVertex]);

                for (unsigned int t = 0; t < pBuffer->getMaxTextureChannelCount(); ++t)
                {
                    CBuffer<QVector3D>& textureCoordBuffer = pBuffer->texCoordBuffer(t);

                    if (m_pAssimpMesh->HasTextureCoords(t))
                    {
                        textureCoordBuffer << CAssimpImporter::aiVector3DToQVector3D(m_pAssimpMesh->mTextureCoords[0][iVertex]);
                    }
                }

                if(m_pAssimpMesh->HasTangentsAndBitangents())
                {
                    vertexTangentBuffer << CAssimpImporter::aiVector3DToQVector3D(m_pAssimpMesh->mTangents[iVertex]);
                    vertexBitangentBuffer << CAssimpImporter::aiVector3DToQVector3D(m_pAssimpMesh->mBitangents[iVertex]);
                }
            }
        }

        if (m_pAssimpMesh->HasFaces())
        {
            CBuffer<IndiceType> indicesBuffer;

            for (unsigned int iFace = 0; iFace < m_pAssimpMesh->mNumFaces; ++iFace)
            {
                int iIndicesCount = m_pAssimpMesh->mFaces[iFace].mNumIndices;

                if (iIndicesCount == 2)
                    pBuffer->setPrimitiveType(ePrimitiveLine);

                for (int iIndice = 0; iIndice < iIndicesCount; ++iIndice)
                {
                    indicesBuffer << m_pAssimpMesh->mFaces[iFace].mIndices[iIndice];
                }
            }

            pBuffer->indicesBuffer() = indicesBuffer;
        }

        if (m_pAssimpMesh->HasBones())
        {
            CBuffer<CBone> bonesBuffer;

            for (unsigned int iBone = 0; iBone < m_pAssimpMesh->mNumBones; ++iBone)
            {
                bonesBuffer << CAssimpImporter::aiBoneToCBone(*m_pAssimpMesh->mBones[iBone]);
            }

            pBuffer->bonesBuffer() = bonesBuffer;

            pBuffer->computeBones();
        }
    }
}

//-----------------------------------------------------------------------------------------
QVector3D CAssimpImporter::aiVector3DToQVector3D(const aiVector3D& v)
{
	return QVector3D(v.x, v.y, v.z);
}

//-----------------------------------------------------------------------------------------
QMatrix4x4 CAssimpImporter::aiMatrix4x4ToQMatrix4x4(const aiMatrix4x4& m)
{
	return QMatrix4x4(m.a1, m.a2, m.a3, m.a4,
					  m.b1, m.b2, m.b3, m.b4,
					  m.c1, m.c2, m.c3, m.c4,
					  m.d1, m.d2, m.d3, m.d4);
}

//-----------------------------------------------------------------------------------------
QQuaternion CAssimpImporter::aiQuaternionToQQuaternion(const aiQuaternion& q)
{
	return QQuaternion(q.w, q.x, q.y, q.z);
}

//-----------------------------------------------------------------------------------------
void CAssimpImporter::aiNodeToCNode(CSceneManager* pSceneManager, const aiNode* pAssimpParentNode, CSceneNode* pParentNode, const QMap<int, CMeshInstance*>& mapMeshID)
{
	for (unsigned int i = 0; i < pAssimpParentNode->mNumChildren; i++)
	{
		aiNode* pAssimpChildNode = pAssimpParentNode->mChildren[i];
		CSceneNode* pChildNode = pParentNode->createChild();

		pChildNode->setName(pAssimpChildNode->mName.C_Str());
		pChildNode->setLocalTransformation(aiMatrix4x4ToQMatrix4x4(pAssimpChildNode->mTransformation));
		pChildNode->setParent(pParentNode);

        aiNodeToCNode(pSceneManager, pAssimpChildNode, pChildNode, mapMeshID);
	}

	for (unsigned int m = 0; m < pAssimpParentNode->mNumMeshes; ++m)
	{
		int iIDMesh = pAssimpParentNode->mMeshes[m];

        if (CMeshInstance* pMeshInstance = mapMeshID[iIDMesh])
		{
            pParentNode->addItem(pMeshInstance);
		}
	}
}

//-----------------------------------------------------------------------------------------
QVector4D CAssimpImporter::aiColor4DToQVector4D(const aiColor4D& c)
{
	return QVector4D(c.r, c.g, c.b, c.a);
}

//-----------------------------------------------------------------------------------------
QVector4D CAssimpImporter::aiColor3DToQVector4D(const aiColor3D& c)
{
	return QVector4D(c.r, c.g, c.b, 1.);
}

//-----------------------------------------------------------------------------------------
EnumLightSourceType CAssimpImporter::aiLightSourceTypeToCLightSourceLightType(aiLightSourceType e)
{
	switch (e)
	{
	case aiLightSource_DIRECTIONAL:
		return eSourceDirectionnal;
		break;
	case aiLightSource_SPOT:
		return eSourceSpot;
		break;
	case aiLightSource_POINT:
	default:
		return eSourcePoint;
		break;
	}
}

//-----------------------------------------------------------------------------------------
CVectorKey CAssimpImporter::aiVectorKeyToCVectorKey(const aiVectorKey& k)
{
	return CVectorKey(k.mTime, aiVector3DToQVector3D(k.mValue)); 
}

//-----------------------------------------------------------------------------------------
CQuaternionKey CAssimpImporter::aiQuatKeyToCQuaternionKey(const aiQuatKey& k)
{
	return CQuaternionKey(k.mTime, aiQuaternionToQQuaternion(k.mValue));
}

//-----------------------------------------------------------------------------------------
CMeshKey CAssimpImporter::aiMeshKeyToAMeshKey(const aiMeshKey& k)
{
	return CMeshKey(k.mTime, k.mValue);
}

//-----------------------------------------------------------------------------------------
CVertexWeight CAssimpImporter::aiVertexWeightToCVertexWeight(const aiVertexWeight& vw)
{
	return CVertexWeight(vw.mVertexId, vw.mWeight);
}

//-----------------------------------------------------------------------------------------
CBone CAssimpImporter::aiBoneToCBone(const aiBone& b)
{
	QString nodeName(b.mName.C_Str());

	CBone bone(nodeName, aiMatrix4x4ToQMatrix4x4(b.mOffsetMatrix));

	QList<CVertexWeight>& vertexWeights = bone.vertexWeights();

	for (unsigned int vw = 0; vw < b.mNumWeights; ++vw)
	{
		vertexWeights << aiVertexWeightToCVertexWeight(b.mWeights[vw]);
	}

	return bone;
}

//-----------------------------------------------------------------------------------------
QList<AEntity*> CAssimpImporter::load(const QString& rootDirectory, const aiScene* pAssimpScene, CSceneManager* pSceneManager, bool bMergeMaterials, bool bMerge, CSceneNode* pNode)
{
    QList<AEntity*> entities;

	//-------------------------------------------------------------------------------------------------
	// TRAITEMENT DES MATERIAUX
	//-------------------------------------------------------------------------------------------------

    QMap<int, QString> mapMaterialName;

	if (pAssimpScene->HasMaterials())
	{
		for (unsigned int m = 0; m < pAssimpScene->mNumMaterials; m++)
		{
			aiMaterial* pAssimpMaterial = pAssimpScene->mMaterials[m];
			CMaterial *pMaterial = 0;

			aiString name;
			aiColor4D diffuse;
			aiColor4D specular;
			aiColor4D ambient;
            float shininess, strength;
			int two_sided;
			int wireframe;
            float opacity;
			unsigned int max = 1;

			int texIndex = 0;
			aiString texPath;	//contains filename of texture

			if (AI_SUCCESS == aiGetMaterialString(pAssimpMaterial, AI_MATKEY_NAME, &name))
			{
                if (bMergeMaterials)
				{
					if (CMaterialManager::getInstance()->isMaterialExist(name.C_Str()))
					{
						pMaterial = CMaterialManager::getInstance()->getMaterialByName(name.C_Str());
                        mapMaterialName[m] = pMaterial->getName();
						continue;
					}
					else
					{
						pMaterial = CMaterialManager::getInstance()->createMaterial(name.C_Str());
                        entities.append(pMaterial);
					}
				}
				else
				{
					pMaterial = CMaterialManager::getInstance()->createMaterial(name.C_Str());
                    entities.append(pMaterial);
				}
			}
			else
			{
				pMaterial = CMaterialManager::getInstance()->createMaterial("Material");
                entities.append(pMaterial);
			}

            mapMaterialName[m] = pMaterial->getName();

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eDiffuse);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_SPECULAR, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eSpecular);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_AMBIENT, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eAmbient);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_EMISSIVE, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eEmissive);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_HEIGHT, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eHeight);
                entities.append(pTexture);
			}
			
			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_NORMALS, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eNormals);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_SHININESS, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eShininess);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_OPACITY, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eOpacity);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_DISPLACEMENT, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
                pMaterial->addTexture(pTexture, eDisplacement);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_LIGHTMAP, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eLightmap);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == pAssimpMaterial->GetTexture(aiTextureType_OPACITY, texIndex, &texPath))
			{
				QFileInfo fileInfo(texPath.C_Str());

                QString fileName = rootDirectory + fileInfo.fileName();
				QString name = fileInfo.baseName();

				CTexture2D* pTexture = CTextureManager::getInstance()->createTexture2D(name, fileName);
				pMaterial->addTexture(pTexture, eReflection);
                entities.append(pTexture);
			}

			if (AI_SUCCESS == aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
			{
				pMaterial->setDiffuseColor(aiColor4DToQVector4D(diffuse));
			}

			if (AI_SUCCESS == aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_SPECULAR, &specular))
			{
				pMaterial->setSpecularColor(aiColor4DToQVector4D(specular));
			}

			if (AI_SUCCESS == aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient))
			{
				pMaterial->setAmbientColor(aiColor4DToQVector4D(ambient));
			}

			int ret1 = aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_SHININESS, &shininess, &max);
			max = 1;
			int ret2 = aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

			if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
			{
				pMaterial->setShininessFactor(shininess * strength);
			}

			max = 1;
			if (AI_SUCCESS == aiGetMaterialIntegerArray(pAssimpMaterial, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
			{
                // TODO
                //pMaterial->setWireFrame((bool)wireframe);
			}

			max = 1;
			if ((AI_SUCCESS == aiGetMaterialIntegerArray(pAssimpMaterial, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
			{
                pMaterial->getRenderPass(0)->renderStates().setFaceCulling(CFaceCulling((bool)two_sided));
			}

			max = 1;
			if (AI_SUCCESS == aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_OPACITY, &opacity, &max))
			{
                pMaterial->setOpacity(opacity);
                if (opacity < 1.)
                {
                    CRenderStates& renderStates = pMaterial->getRenderPass(0)->renderStates();
                    renderStates.blending().setEnabled(true);
                    renderStates.blending().setBlendFunction(Source::eDestinationColor, Destination::eSourceColor);
                    renderStates.faceCulling().setEnabled(false);
                    renderStates.setDepthMask(false);
                }
			}
		}
	}

	//-------------------------------------------------------------------------------------------------
	// TRAITEMENT DES LIGHTS
	//-------------------------------------------------------------------------------------------------

	if (pAssimpScene->HasLights())
	{
		for (unsigned int l = 0; l < pAssimpScene->mNumLights; l++)
		{
			aiLight* pAssimpLight = pAssimpScene->mLights[l];
			CLight *pLight = pSceneManager->createLight(pAssimpLight->mName.C_Str());

			pLight->setSourceType(aiLightSourceTypeToCLightSourceLightType(pAssimpLight->mType));
			pLight->setDirection(aiVector3DToQVector3D(pAssimpLight->mDirection));
			pLight->setConstantAttenuation(pAssimpLight->mAttenuationConstant);
			pLight->setLinearAttenuation(pAssimpLight->mAttenuationLinear);
			pLight->setQuadraticAttenuation(pAssimpLight->mAttenuationQuadratic);
			pLight->setDiffuseColor(aiColor3DToQVector4D(pAssimpLight->mColorDiffuse));
			pLight->setSpecularColor(aiColor3DToQVector4D(pAssimpLight->mColorSpecular));
			pLight->setAmbientColor(aiColor3DToQVector4D(pAssimpLight->mColorAmbient));
			pLight->setInnerConeAngle(pAssimpLight->mAngleInnerCone);
			pLight->setOuterConeAngle(pAssimpLight->mAngleOuterCone);
            entities.append(pLight);
		}
	}

	//-------------------------------------------------------------------------------------------------
	// TRAITEMENT DES CAMERAS
	//-------------------------------------------------------------------------------------------------

	if (pAssimpScene->HasCameras())
	{
		for (unsigned int c = 0; c < pAssimpScene->mNumCameras; c++)
		{
			aiCamera* pAssimpCamera = pAssimpScene->mCameras[c];
			CCamera* pCamera = pSceneManager->createCamera(pAssimpCamera->mName.C_Str());
			pCamera->setEyePosition(aiVector3DToQVector3D(pAssimpCamera->mPosition));
			pCamera->setUp(aiVector3DToQVector3D(pAssimpCamera->mUp));
			pCamera->setCenter(aiVector3DToQVector3D(pAssimpCamera->mLookAt));
			pCamera->setHorizontalFOV(pAssimpCamera->mHorizontalFOV);
			pCamera->setNear(pAssimpCamera->mClipPlaneNear);
			pCamera->setFar(pAssimpCamera->mClipPlaneFar);
			pCamera->setAspectRatio(pAssimpCamera->mAspect);
            entities.append(pCamera);
		}
	}

	//-------------------------------------------------------------------------------------------------
	// TRAITEMENT DES ANIMATIONS
	//-------------------------------------------------------------------------------------------------

	if (pAssimpScene->HasAnimations())
	{
		for (unsigned int a = 0; a < pAssimpScene->mNumAnimations; a++)
		{
			aiAnimation* pAssimpAnimation = pAssimpScene->mAnimations[a];
			CAnimation* pAnimation = pSceneManager->createAnimation(pAssimpAnimation->mName.C_Str());
            entities.append(pAnimation);
			pAnimation->setDuration(pAssimpAnimation->mDuration);
			pAnimation->setTicksPerSecond(pAssimpAnimation->mTicksPerSecond);
			
			for (unsigned int iChannel = 0; iChannel  < pAssimpAnimation->mNumChannels; ++iChannel)
			{
				aiNodeAnim* pAssimpNodeAnim = pAssimpAnimation->mChannels[iChannel];
				CSceneNodeAnimation* pNodeAnimation = pAnimation->createNodeAnimation(pAssimpNodeAnim->mNodeName.C_Str());
				
				QList<CVectorKey>& posKeys = pNodeAnimation->positionKeys();

				for (unsigned int pK = 0; pK < pAssimpNodeAnim->mNumPositionKeys; ++pK)
				{
					posKeys << aiVectorKeyToCVectorKey(pAssimpNodeAnim->mPositionKeys[pK]);
				}

				QList<CQuaternionKey>& rotKeys = pNodeAnimation->rotationKeys();

				for (unsigned int rK = 0; rK < pAssimpNodeAnim->mNumRotationKeys; ++rK)
				{
					rotKeys << aiQuatKeyToCQuaternionKey(pAssimpNodeAnim->mRotationKeys[rK]);
				}

				QList<CVectorKey>& scKeys = pNodeAnimation->scalingKeys();

				for (unsigned int sK = 0; sK < pAssimpNodeAnim->mNumScalingKeys; ++sK)
				{
					scKeys << aiVectorKeyToCVectorKey(pAssimpNodeAnim->mScalingKeys[sK]);
				}
			}

			for (unsigned int iMeshChannel = 0; iMeshChannel  < pAssimpAnimation->mNumMeshChannels; ++iMeshChannel)
			{
				aiMeshAnim* pAssimpMeshAnim = pAssimpAnimation->mMeshChannels[iMeshChannel];
				CMeshAnimation* pMeshAnimation = pAnimation->createMeshAnimation(pAssimpMeshAnim->mName.C_Str());

				QList<CMeshKey>& keys = pMeshAnimation->keys();

				for (unsigned int k = 0; k < pAssimpMeshAnim->mNumKeys; ++k)
				{
					keys << aiMeshKeyToAMeshKey(pAssimpMeshAnim->mKeys[k]);
				}
			}
		}
	}

    //-------------------------------------------------------------------------------------------------
    // TRAITEMENT DES MESHS
    //-------------------------------------------------------------------------------------------------
    if (bMerge)
    {
        QMap<int, CMeshInstance*> mapMeshID;

        aiNode* pAssimpRootNode = pAssimpScene->mRootNode;

        if (!pNode)
        {
            pNode = pSceneManager->getRootNode();
        }

        CSceneNode* pSceneRootNode = pNode->createChild(pAssimpRootNode->mName.C_Str());

        if (pAssimpScene->HasMeshes())
        {
            for (unsigned int iMesh = 0; iMesh < pAssimpScene->mNumMeshes; ++iMesh)
            {
                aiMesh* pAssimpMesh = pAssimpScene->mMeshes[iMesh];
                CAssimpMesh* pMesh = CMeshManager::getInstance()->createCustomMesh<CAssimpMesh>("AssimpMesh", pAssimpMesh->mName.C_Str());
                entities.append(pMesh);
                pMesh->setAiMesh(pAssimpMesh);
                pMesh->setMaterialName(mapMaterialName[pAssimpMesh->mMaterialIndex]);
                CMeshInstance* pMeshInstance = pSceneManager->createMeshInstance(pMesh, pMesh->getName() + "_Instance");
                pMeshInstance->setLocalRootNode(pSceneRootNode);
                mapMeshID[iMesh] = pMeshInstance;
                entities.append(pMeshInstance);
            }
        }

        //-------------------------------------------------------------------------------------------------
        // TRAITEMENT DES NODES
        //-------------------------------------------------------------------------------------------------

        pSceneRootNode->setLocalTransformation(aiMatrix4x4ToQMatrix4x4(pAssimpRootNode->mTransformation));
        aiNodeToCNode(pSceneManager, pAssimpRootNode, pSceneRootNode, mapMeshID);
    }
    else
    {
        if (pAssimpScene->HasMeshes())
        {
            for (unsigned int iMesh = 0; iMesh < pAssimpScene->mNumMeshes; ++iMesh)
            {
                aiMesh* pAssimpMesh = pAssimpScene->mMeshes[iMesh];
                CAssimpMesh* pMesh = CMeshManager::getInstance()->createCustomMesh<CAssimpMesh>("AssimpMesh", pAssimpMesh->mName.C_Str());
                pMesh->setAiMesh(pAssimpMesh);
                pMesh->setMaterialName(mapMaterialName[pAssimpMesh->mMaterialIndex]);
                entities.append(pMesh);
                CMeshInstance* pMeshInstance = pSceneManager->createMeshInstance(pMesh, pMesh->getName() + "_Instance");
                entities.append(pMeshInstance);
            }
        }
    }

    return entities;
}

//-----------------------------------------------------------------------------------------
QList<AEntity*> CAssimpImporter::mergeScene(const QString& fileName, CSceneManager* pSceneManager, bool bMergeMaterials, CSceneNode* pNode /*= 0*/)
{
    QList<AEntity*> entities;
    Assimp::Importer importer;

    if (!fileName.isEmpty())
	{
        const aiScene* pAssimpScene = importer.ReadFile(fileName.toStdString().c_str(), aiProcessPreset_TargetRealtime_Quality);

        if (pAssimpScene)
        {
            entities = load(QFileInfo(fileName).absolutePath() + "/", pAssimpScene, pSceneManager, bMergeMaterials, true, pNode);
            pLog->addMessage(eINFO, QString("AssimpImporter: %1 : merging %2 entities.").arg(fileName).arg(entities.size()));
        }
        else
        {
            pLog->addMessage(eERROR, QString("AssimpImporter: %1 : error while loading.").arg(fileName));
        }
    }
    else
    {
        pLog->addMessage(eERROR, QString("AssimpImporter: %1 : file name is empty.").arg(fileName));
	}

    return entities;
}

//-----------------------------------------------------------------------------------------
QList<AEntity*> CAssimpImporter::loadScene(const QString& fileName, CSceneManager* pSceneManager, bool bMergeMaterials)
{
    QList<AEntity*> entities;
    Assimp::Importer importer;

    if (!fileName.isEmpty())
    {
        const aiScene* pAssimpScene = importer.ReadFile(fileName.toStdString().c_str(), aiProcessPreset_TargetRealtime_Quality);

        if (pAssimpScene)
        {
            entities = load(QFileInfo(fileName).absolutePath() + "/", pAssimpScene, pSceneManager, bMergeMaterials, false, 0);
            pLog->addMessage(eINFO, QString("AssimpImporter: %1: loading %2 entities.").arg(fileName).arg(entities.size()));
        }
        else
        {
            pLog->addMessage(eERROR, QString("AssimpImporter: %1 : error while loading.").arg(fileName));
        }
	}
    else
    {
        pLog->addMessage(eERROR, QString("AssimpImporter: %1 : file name is empty.").arg(fileName));
    }

    return entities;
}

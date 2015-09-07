package opengl.renderer;

import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import org.lwjgl.opengl.GL11;
import org.lwjgl.opengl.GL13;

import math.QSize;
import math.matrices.*;
import math.vector.QVector3D;
import math.vector.QVector4D;
import opengl.buffers.GLMeshBuffer;
import opengl.shaders.GLShaderProgram;
import opengl.textures.AGLTexture;
import opengl.textures.GLTexture2D;
import opengl.textures.GLTextureCube;
import opengl.textures.GLTextureTarget;
import tools.log.LogManager;
import tools.log.EnumLogType;
import tools.types.MultiMap;
import geometry.animation.AnimationManager;
import geometry.buffers.MeshBuffer;
import geometry.camera.Camera;
import geometry.light.Light;
import geometry.material.EnumMaterialParameter;
import geometry.material.Material;
import geometry.material.MaterialManager;
import geometry.material.MaterialTextureParameter;
import geometry.renderer.ARenderer;
import geometry.renderer.RenderQueue;
import geometry.renderstates.EnumPrimitiveType;
import geometry.renderstates.EnumRasterizationMode;
import geometry.renderstates.RenderStates;
import geometry.scenegraph.ARenderableItem;
import geometry.scenegraph.SceneManager;
import geometry.scenegraph.SceneNode;
import geometry.shaders.EnumShadingModel;
import geometry.shaders.Shader;
import geometry.textures.ATextureDescription;
import geometry.textures.ImageRect;
import geometry.textures.TextureDescription2D;
import geometry.textures.TextureDescriptionCube;
import geometry.textures.TextureManager;

class GLRenderer extends ARenderer 
	implements SceneManager.IShaderListener, SceneManager.IMeshBufferListener, SceneManager.ICameraListener, TextureManager.IListener, AnimationManager.IListener
{
	
	//! Constructeur
	GLRenderer(SceneManager pSceneManager,
			AnimationManager pAnimationManager) {
		super(pSceneManager, pAnimationManager);

		m_pCurrentShader = null;
		m_iDrawCalls = 0;
		m_iPolygonsPerFrame = 0;
		
		setName("OpenGL Renderer - Java Desktop Version");

		LogManager.getInstance().addMessage(EnumLogType.eINFO, "Creating OpenGL Renderer - Desktop Version");
		
		pSceneManager.registerShaderListener(this);
		pSceneManager.registerCameraListener(this);
		pSceneManager.registerMeshBufferListener(this);
		TextureManager.getInstance().registerListener(this);
		pAnimationManager.registerListener(this);
	}

	//! Destructeur
	void finalize()
	{
		m_pSceneManager.unregisterShaderListener(this);
		m_pSceneManager.unregisterShaderListener(this);
		m_pSceneManager.unregisterMeshBufferListener(this);
		TextureManager.getInstance().unregisterListener(this);
		m_pAnimationManager.unregisterListener(this);

		for (AGLTexture pTexture : m_Textures.values())
		{
			pTexture.delete();
		}

		m_Textures.clear();

	    for (GLShaderProgram pShaderProgram : m_ShaderPrograms.values())
		{
	    	pShaderProgram.delete();
		}

		m_ShaderPrograms.clear();
	}
	
	//--------------------------------------------------------------------------------------------
	// Implémentation ARenderer
	//--------------------------------------------------------------------------------------------

	
	//! Override
	void render() {
		if (m_pSceneManager == null)
			return;
		
	    if (m_bEnabled && m_bIsDirty)
		{
			m_iDrawCalls = 0;
			m_iPolygonsPerFrame = 0;
			m_iMaterialBind = 0;
			m_iShaderBind = 0;
			
		
	        // Efface le buffer GL
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			////////////////////////////////////////////////////////////////////////////
			// Rendu des ARenderableItem
			////////////////////////////////////////////////////////////////////////////

			RenderQueue renderQueue = m_pSceneManager.getRenderQueue();
			Set<Integer> materialIDs = renderQueue.materialIDs();

			////////////////////////////////////////////////////////////////////////////
			// Items opaques
			////////////////////////////////////////////////////////////////////////////

	        MultiMap<GLShaderProgram, Integer> shaderQueue = new MultiMap<GLShaderProgram, Integer>(); 

			// La render queue est triée par matériaux. L'objectif ici est de trier les matériaux par shader afin de minimiser au maximum
			// le chargement / déchargement de shader sur la carte graphique en vue d'augmenter les performances
			for (int iMaterialID : materialIDs)
			{
				Material pMaterial = MaterialManager.getInstance().getMaterialByID(iMaterialID);
				if (pMaterial != null)
				{
	                GLShaderProgram pShaderProgram = getShaderProgram(pMaterial);

					if (pShaderProgram != null)
					{
						shaderQueue.insert(pShaderProgram, iMaterialID);
					}
				}
			}
			
			renderItems(renderQueue, shaderQueue);

			RenderQueue renderTransparentQueue = m_pSceneManager.getTransparentRenderQueue();
			Set<Integer> transparentMaterialIDs = renderTransparentQueue.materialIDs();

			////////////////////////////////////////////////////////////////////////////
			// Items transparents
			////////////////////////////////////////////////////////////////////////////

			MultiMap<GLShaderProgram, Integer> transparentShaderQueue = new MultiMap<GLShaderProgram, Integer>();

			// La render queue est triée par matériaux. L'objectif ici est de trier les matériaux par shader afin de minimiser au maximum
			// le chargement / déchargement de shader sur la carte graphique en vue d'augmenter les performances
			for (int iMaterialID : transparentMaterialIDs)
			{
				Material pMaterial = MaterialManager.getInstance().getMaterialByID(iMaterialID);
				if (pMaterial != null)
				{
					GLShaderProgram pShaderProgram = getShaderProgram(pMaterial);

					if (pShaderProgram != null)
					{
						transparentShaderQueue.insert(pShaderProgram, iMaterialID);
					}
				}
			}

			renderItems(renderTransparentQueue, transparentShaderQueue);

	        m_bIsDirty = false;
	    }
	}

	//! Override
	void renderMeshBuffer(MeshBuffer pBuffer, RenderStates renderStates) {
		if (m_pSceneManager == null)
			return;

		if (!isInit())
			return;

		if (!m_HardwareBufferMap.containsKey(pBuffer))
		{
			createVertexBufferObject(pBuffer);
		}
		else if (pBuffer.isDirty())
		{
			updateVertexBufferObject(pBuffer);
		}

		renderVertexBufferObject(pBuffer, renderStates);
	}

	//! Override
	void renderMeshBuffer(List<MeshBuffer> buffers,
			RenderStates renderStates) {
		if (m_pSceneManager == null)
			return;

		if (!isInit())
			return;

		for (MeshBuffer pBuffer : buffers)
		{
			if (!m_HardwareBufferMap.containsKey(pBuffer))
			{
				createVertexBufferObject(pBuffer);
			}
			else if (pBuffer.isDirty())
			{
				updateVertexBufferObject(pBuffer);
			}

			renderVertexBufferObject(pBuffer, renderStates);
		}
	}

	//! Override
	void renderToTexture(TextureDescription2D pTexture, ARenderableItem pItem) {
		
		AGLTexture pGLTexture = getTexture(pTexture);
		
		if (pGLTexture != null)
		{
			pGLTexture.bind();
			if (pItem.isVisible())
			{
				Material pMaterial = pItem.getMaterial();
				
				if (pMaterial != null)
				{
					m_pCurrentShader = getShaderProgram(pMaterial);

					m_pCurrentShader.bind();

					bindCamera();
					bindTime();
					bindMaterial(pMaterial);
					bindLights();

					List<SceneNode> nodes = pItem.getNodes();

					for (SceneNode pNode : nodes)
					{
						bindNode(pNode);
	                    pItem.doRender(this);
					}

					releaseMaterial(pMaterial);
					m_pCurrentShader.release();
				}
			}
			pGLTexture.release();
		}
		
	}

	//! Override
	void renderToTexture(TextureDescription2D pTexture, List<ARenderableItem> items) {
		AGLTexture pGLTexture = getTexture(pTexture);
		
		if (pGLTexture != null)
		{
			pGLTexture.bind();
			for (ARenderableItem pItem : items)
			{
				if (pItem.isVisible())
				{
					Material pMaterial = pItem.getMaterial();
					
					if (pMaterial != null)
					{
						m_pCurrentShader = getShaderProgram(pMaterial);

						m_pCurrentShader.bind();

						bindCamera();
						bindTime();
						bindMaterial(pMaterial);
						bindLights();

						List<SceneNode> nodes = pItem.getNodes();

						for (SceneNode pNode : nodes)
						{
							bindNode(pNode);
	                        pItem.doRender(this);
						}

						releaseMaterial(pMaterial);
						m_pCurrentShader.release();
					}
				}
			}
			pGLTexture.release();
		}
		
	}

	//--------------------------------------------------------------------------------------------
	// Fin implémentation ARenderer
	//--------------------------------------------------------------------------------------------

	//! Override - Définit le mode de remplissage
	void setRasterizationMode(EnumRasterizationMode eRasterizationMode)
	{
		// Gestion en fonction de l'état précédent
		switch (m_eRasterizationMode)
		{
			case eLine:
			{
				glEnable(GL_CULL_FACE);
			}
			break;

	        case eFill:
	        case ePoint:
	        default:
	            break;
		}

		// Gestion en fonction de l'état courant
		switch (eRasterizationMode)
		{
			case eLine:
			{
				glDisable(GL_CULL_FACE);
			}
			break;

			case ePoint:
			{
	            //glPointSize(4.0);
			}

	        case eFill:
	        default:
	            break;
		}

		super.setRasterizationMode(eRasterizationMode);
	}

	//! Retourne le nombre de draw call par frame
	int getDrawCallCount() { return m_iDrawCalls; }

	//! Retourne le nombre de polygones par frame
	int getPolygonCount() { return m_iPolygonsPerFrame; }
	
	//! Conteneur associatif qui permet de trouver un buffer GPU décrivant un item
    HashMap<MeshBuffer, GLMeshBuffer> m_HardwareBufferMap = new HashMap<MeshBuffer, GLMeshBuffer>();

    //! Shader program
    HashMap<String, GLShaderProgram> m_ShaderPrograms = new HashMap<String, GLShaderProgram>(); 

	//! Conteneur de texture
    HashMap<String, AGLTexture> m_Textures = new HashMap<String, AGLTexture>();

	//--------------------------------------------------------------------------------------------
	// Listeners
	//--------------------------------------------------------------------------------------------

	//! Implémentation - Interface CTextureManager::IListener
	void onUpdate(ATextureDescription pTexture)
	{
		if (pTexture.isValid())
		{
			addTexture(pTexture);
		}
	}

	//! Implémentation - Interface CTextureManager::IListener
	void onDelete(ATextureDescription pTexture)
	{
		String textureName = pTexture.getName();

		if (m_Textures.containsKey(textureName))
		{
			m_Textures.remove(textureName);
		}
	}

	//! Implémentation - Interface CSceneManager::IListener
	void onDelete(MeshBuffer pBuffer)
	{
		if (m_HardwareBufferMap.containsKey(pBuffer))
		{
			GLMeshBuffer pGPUBuffer = m_HardwareBufferMap.get(pBuffer);
			m_HardwareBufferMap.remove(pBuffer);
			LogManager.getInstance().addMessage(EnumLogType.eDEBUGMEMORY, "Suppression GPU Buffer");
			pGPUBuffer.delete();
	
			// La destruction du buffer induit la libération de la mémoire occupé sur le serveur OpenGL
		}
	}

	//! Implémentation - Interface CSceneManager::IListener
	void onUpdate(Shader pShader)
	{
	    if (!isInit())
			return;

		if (m_ShaderPrograms.containsKey(pShader.getName()))
		{
			// Le shader existe déjà ? on remplace le précédent
			m_ShaderPrograms.get(pShader.getName()).delete();
		}

		bindShader(pShader);
	}

	//! Implémentation - Interface CSceneManager::IListener
	void onDelete(Shader pShader)
	{
	    if (!isInit())
			return;

		if (m_ShaderPrograms.containsKey(pShader.getName()))
		{
			LogManager.getInstance().addMessage(EnumLogType.eINFO, "Suppression du shader: " + pShader.getName());

	        GLShaderProgram pProgram = m_ShaderPrograms.get(pShader.getName());

			m_ShaderPrograms.remove(pShader.getName());

			// Le shader existe déjà ? on remplace le précédent
			pProgram.delete();
		}
	}


    //! Implémentation - Interface CSceneManager::IListener
    void onUpdate(Camera pC)
    {
        setDirty();
    }


	//! Implémentation - Interface CAnimationManager::IListener
    void onUpdate()
    {
        setDirty();
    }

	//--------------------------------------------------------------------------------------------
	// Méthodes de rendu
	//--------------------------------------------------------------------------------------------

	//! Rendu des items
	void renderItems(RenderQueue renderQueue, MultiMap<GLShaderProgram, Integer> shaderQueue)
	{
		Set<GLShaderProgram> shaderPrograms = shaderQueue.keySet();

		for (GLShaderProgram pShaderProgram : shaderPrograms)
		{
			m_pCurrentShader = pShaderProgram;
			m_pCurrentShader.bind();

			bindNode(m_pSceneManager.getRootNode());
			
					
			List<Integer> materialIDs = shaderQueue.get(pShaderProgram);

			for (int iMaterialID : materialIDs)
			{
				Material pMaterial = MaterialManager.getInstance().getMaterialByID(iMaterialID);
				
				if (pMaterial != null)
				{
					List<ARenderableItem> items = renderQueue.getItemsByMaterialID(iMaterialID);

					if (!items.isEmpty() && isOneVisible(items))
					{
						bindCamera();
						bindTime();
						bindMaterial(pMaterial);
						bindLights();

						for (ARenderableItem pItem : items)
						{
							if (pItem.isVisible())
							{
								List<SceneNode> nodes = pItem.getNodes();

								for (SceneNode pNode : nodes)
								{
									bindNode(pNode);
									pItem.doRender(this);
								}
							}
						}

						releaseMaterial(pMaterial);
					}
				}
			}

			m_pCurrentShader.release();
		}
	}

	//! Rendu des meshs en fonction de leur VBO
    void renderVertexBufferObject(MeshBuffer pBuffer, RenderStates renderParams)
    {
    	m_iDrawCalls++;

    	// BONES
    	m_pCurrentShader.setUniformValue("hasSkeleton", pBuffer.hasSkeleton());

    	if (pBuffer.hasSkeleton())
    	{
    		// TODO
    		List<QMatrix4x4> bones = pBuffer.getBoneTransformations(m_pSceneManager.getRootNode());
    		//m_pCurrentShader.setUniformValueArray("bones", bones.toBuffer());
    	}


    	
    	EnumPrimitiveType ePrimitiveType = pBuffer.getPrimitiveType();

    	bool bDisableCullFace = !renderParams.getFaceCulling().isEnabled() || ePrimitiveType ==  EnumPrimitiveType.ePrimitiveLine;

    	if (!renderParams.getDepthTest().isEnabled()) glDisable(GL_DEPTH_TEST);
    	if (bDisableCullFace) glDisable(GL_CULL_FACE);

    	m_iPolygonsPerFrame += pBuffer.getFaceCount();
    	
    	
    	GLMeshBuffer pGPUBuffer = m_HardwareBufferMap.get(pBuffer);
    	pGPUBuffer.setPrimitiveType(ePrimitiveType.fromRasterizationMode(m_eRasterizationMode));

    	pGPUBuffer.draw(m_pCurrentShader);
    	
    	if (bDisableCullFace) glEnable(GL_CULL_FACE);
    	if (!renderParams.getDepthTest().isEnabled()) glEnable(GL_DEPTH_TEST);
    }

	//! Lie les paramétres temporels
	void bindTime()
	{
		if (m_pSceneManager == null)
			return;

		m_pCurrentShader.setUniformValue("iGlobalTime", (float) (m_Time.getTimeInMillis() / 1000.));
	}


	//! Lie les paramétres de camera (modéle, vue, projection, normal...) au shader
	void bindCamera()
	{
		if (m_pSceneManager == null)
			return;

		// On récupére la caméra courante
		Camera pCamera = m_pSceneManager.getCurrentCamera();
		if (pCamera != null)
		{
			QMatrix4x4 projectionMatrix = pCamera.getProjectionMatrix();
			m_pCurrentShader.setUniformValue("projectionMatrix", projectionMatrix);

			QVector3D eyePosition = pCamera.getEyePosition();
			m_pCurrentShader.setUniformValue("eyePosition", eyePosition);
		}
	}
	

	//! Lie les matrices du noeud au shader
    void bindNode(SceneNode pNode)
    {
    	if (m_pSceneManager == null)
    		return;

    	// On récupére la caméra courante
    	Camera pCamera = m_pSceneManager.getCurrentCamera();
    	if (pCamera != null)
    	{
    		// On concaténe les matrices pour obtenir la matrice "Model-Vue-Projection" qui correspond
    		// au changement de repére pour passer du répére de l'objet à celui de la caméra
    		QMatrix4x4 modelViewProjectionMatrix = QMatrix4x4.mul(pCamera.getTransformation(), pNode.getGlobalTransformation());
    		m_pCurrentShader.setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);

    		QMatrix4x4 modelViewMatrix = QMatrix4x4.mul(pCamera.getViewMatrix(), pNode.getGlobalTransformation());
    		m_pCurrentShader.setUniformValue("modelViewMatrix", modelViewMatrix);

    		QMatrix4x4 viewInverseMatrix = QMatrix4x4.mul(pCamera.getViewMatrix().inverted(), pNode.getGlobalTransformation());
    		m_pCurrentShader.setUniformValue("viewInverseMatrix", viewInverseMatrix);

    		// La "normal matrix" est définie comme "la transposée de l'inverse de la matrice modéle-vue"
    		QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
    		m_pCurrentShader.setUniformValue("normalMatrix", normalMatrix);
    	}
    }

	//! Lie les attributs des lumiéres au shader
    void bindLights()
    {
    	if (m_pSceneManager == null)
    		return;

    	Camera pCamera = m_pSceneManager.getCurrentCamera();

    	if (pCamera == null)
    		return;


    	////////////////////////////////////////////////////////////////////////////////////////////
    	// TRAITEMENTS DES LUMIERES
    	////////////////////////////////////////////////////////////////////////////////////////////

    	m_pCurrentShader.setUniformValue("sceneAmbient", m_pSceneManager.getAmbientColor());
    	m_pCurrentShader.setUniformValue("lightCount", m_pSceneManager.getLightCount());

    	int iLightID = 0;

    	for (Light pLight : m_pSceneManager.getLights())
    	{
    		SceneNode pLightNode = pLight.getNode();
    		
    		if (pLightNode != null)
    		{
    			// On calcul la position de la lumière dans le repère de l'oeil
    			QVector4D lightPosition = QMatrix4x4.mul(QMatrix4x4.mul(pCamera.getViewMatrix(), pLightNode.getGlobalTransformation()), new QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    			m_pCurrentShader.setUniformValue(String.format("lights[%d].enabled", 				iLightID), pLight.isEnabled());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].position", 				iLightID), lightPosition);
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].direction", 				iLightID), pLight.getDirection());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].ambient", 				iLightID), pLight.getAmbientColor());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].diffuse", 				iLightID), pLight.getDiffuseColor());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].specular", 				iLightID), pLight.getSpecularColor());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].constantAttenuation", 	iLightID), pLight.getConstantAttenuation());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].linearAttenuation", 		iLightID), pLight.getLinearAttenuation());
    			m_pCurrentShader.setUniformValue(String.format("lights[%d].quadraticAttenuation", 	iLightID), pLight.getQuadraticAttenuation());	

    			iLightID++;
    		}
    	}
    }

	//! Lie l'attribut d'un materiau au shader
    void bindMaterial(Material pMaterial)
    {
    	m_iMaterialBind++;

    	m_pCurrentShader.setUniformValue("material.ambient",		pMaterial.getAmbientColor());
    	m_pCurrentShader.setUniformValue("material.diffuse",		pMaterial.getDiffuseColor());
    	m_pCurrentShader.setUniformValue("material.specular",		pMaterial.getSpecularColor());
    	m_pCurrentShader.setUniformValue("material.alphaMask",		pMaterial.getAlphaMaskColor());
    	m_pCurrentShader.setUniformValue("material.alphaMaskFunc",	pMaterial.getAlphaMaskFunc().ordinal());
    	m_pCurrentShader.setUniformValue("material.shininess",		pMaterial.getShininessFactor());
    	m_pCurrentShader.setUniformValue("material.isWireFrame",	pMaterial.isWireFrame());
    	m_pCurrentShader.setUniformValue("material.isTwoSided",		pMaterial.isTwoSided());
    	m_pCurrentShader.setUniformValue("material.opacity",		pMaterial.getOpacity());

    	linkUserUniformValues(pMaterial);


    	if (pMaterial.isBlendingEnabled())
    	{
    		glEnable(GL_BLEND);
    		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    	}

    	if (pMaterial.isTwoSided())
    		glDisable(GL_CULL_FACE);
    	else 
    		glEnable(GL_CULL_FACE);

    	if (pMaterial.isTextured())
    	{
    		HashMap<EnumMaterialParameter, Integer> materialParameterCount = new HashMap<EnumMaterialParameter, Integer>();
    		materialParameterCount.put(EnumMaterialParameter.eDiffuse, 		0);
    		materialParameterCount.put(EnumMaterialParameter.eSpecular, 	0);
    		materialParameterCount.put(EnumMaterialParameter.eAmbient, 		0);
    		materialParameterCount.put(EnumMaterialParameter.eEmissive, 	0);
    		materialParameterCount.put(EnumMaterialParameter.eHeight, 		0);
    		materialParameterCount.put(EnumMaterialParameter.eNormals, 		0);
    		materialParameterCount.put(EnumMaterialParameter.eShininess, 	0);
    		materialParameterCount.put(EnumMaterialParameter.eOpacity, 		0);
    		materialParameterCount.put(EnumMaterialParameter.eDisplacement, 0);
    		materialParameterCount.put(EnumMaterialParameter.eLightmap, 	0);
    		materialParameterCount.put(EnumMaterialParameter.eReflection, 	0);
    		
    		for (MaterialTextureParameter texture : pMaterial.textures())
    		{
    			ATextureDescription pTexture = TextureManager.getInstance().getTextureByID(texture.getTextureID());
    			if (pTexture != null)
    			{
    				EnumMaterialParameter eType = texture.getMaterialParameter();
    				int iUnit = pTexture.getTextureUnit();

    				switch (pTexture.getType())
    				{
    				case eTextureDescription2D:
    					{
    						AGLTexture pGLTexture = getTexture(pTexture);
    						if (pGLTexture != null)
    						{
    							if (pGLTexture.isValid())
    							{
    								GL13.glActiveTexture(GL13.GL_TEXTURE0 + iUnit);
    								//m_pCurrentShader.setUniformValue(String.format("%1%2").arg(pGLHelper.fromType(eType)).arg(materialParameterCount[eType]).toLatin1().constData(), iUnit);
    								pGLTexture.bind();
    								materialParameterCount.put(eType, materialParameterCount.get(eType) + 1);
    							}
    						}
    					}
    					break;

    				case eTextureDescription3D:
    					{
    						// TODO
    					}
    					break;

    				case eTextureDescriptionCube:
    					{
    						AGLTexture pGLTexture = getTexture(pTexture);
    						if (pGLTexture != null)
    						{
    							if (pGLTexture.isValid())
    							{
    								GL13.glActiveTexture(GL13.GL_TEXTURE0 + iUnit);
    								m_pCurrentShader.setUniformValue("cubeMapTex", iUnit);
    								pGLTexture.bind();
    							}
    						}
    					}
    					break;

    				case eTextureDescriptionUndefined:
    				default:
    					{
    					}
    					break;
    				}
    			}
    		}

    		/*
    		for (Map.Entry<EnumMaterialParameter, Integer> entry : materialParameterCount.entrySet())
    		{
    			it.next();
    			m_pCurrentShader.setUniformValue(pGLHelper.fromTypeCount(it.key()).toLatin1().constData(), it.value());	
    		}*/
    	}
    }

	//! Lie un shader
	void bindShader(Shader pShader)
	{
		m_iShaderBind++;

		LogManager.getInstance().addMessage(EnumLogType.eINFO, "Linking shader" + pShader.getName() + "...");

		GLShaderProgram pNewShaderProgram = new GLShaderProgram(
				pShader.getVertexShaderFileName(),
				pShader.getGeometryShaderFileName(),
				pShader.getFragmentShaderFileName());

		m_ShaderPrograms.put(pShader.getName(), pNewShaderProgram);

		//emit newLogMessage(bIsOnError ? eERROR : eINFO, log);

		pNewShaderProgram.link();
	}

	//! Crée un VBO en fonction d'un VertexBuffer
	void createVertexBufferObject(MeshBuffer pBuffer)
	{
		if (m_pSceneManager == null)
			return;

	    if (!isInit())
			return;

		GLMeshBuffer pGPUBuffer = new GLMeshBuffer(pBuffer);
		
		m_HardwareBufferMap.put(pBuffer, pGPUBuffer);

		pBuffer.wash();
	}

	//! Met à jour un VBO
	void updateVertexBufferObject(MeshBuffer pBuffer)
	{
		if (m_pSceneManager == null)
			return;	

		if (!isInit())
			return;

		GLMeshBuffer pGPUBuffer = m_HardwareBufferMap.get(pBuffer);
		pGPUBuffer.update(pBuffer);
		
		pBuffer.wash();	
	}

	//! Ajoute une texture 2D
	void addTexture(ATextureDescription pTexture)
	{
	    if (!isInit())
			return;

		if (m_Textures.containsKey(pTexture.getName())) // la texture existe deja ?
		{
			LogManager.getInstance().addMessage(EnumLogType.eDEBUG, "CGLRenderer : " + pTexture.getName() + " suppression pour mise à jour.");
			AGLTexture pGLTexture = m_Textures.get(pTexture.getName());
			m_Textures.remove(pTexture.getName());
			pGLTexture.delete();;
		}

		AGLTexture pGLTexture = null;

		if (pTexture instanceof TextureDescription2D)
		{
			TextureDescription2D pTexture2D = (TextureDescription2D) pTexture;
			bool bIsEmpty = pTexture2D.imageRects().size() == 0;

			QSize texSize = pTexture2D.getSize();

			if (bIsEmpty)
			{
				if (pTexture2D.isTextureTarget())
				{
					pGLTexture = new GLTexture2D(texSize.width(), texSize.height());
				}
				else
				{
					pGLTexture = new GLTextureTarget(texSize.width(), texSize.height());
				}
			}
			else
			{
				GLTexture2D pGLTexture2D = new GLTexture2D(texSize.width(), texSize.height());
				pGLTexture = pGLTexture2D;

				for (ImageRect imgRect : pTexture2D.imageRects())
				{
					pGLTexture2D.loadSub(imgRect.getName(), imgRect.getRect());
				}
			}
		}
		else if (pTexture instanceof TextureDescriptionCube)
		{
			TextureDescriptionCube pTextureCube = (TextureDescriptionCube)pTexture;
			pGLTexture = new GLTextureCube(pTextureCube.getFileNames());
		}
		
		if (pGLTexture != null)
		{
			m_Textures.put(pTexture.getName(), pGLTexture);

			if (!pGLTexture.isValid())
			{
				LogManager.getInstance().addMessage(EnumLogType.eWARN, "CGLRenderer::addTexture() : Erreur chargement de la texture : " + pTexture.getName());
			}
			else
			{		
				LogManager.getInstance().addMessage(EnumLogType.eDEBUG, "CGLRenderer : " + pTexture.getName() + " chargee avec succes.");
				LogManager.getInstance().addMessage(EnumLogType.eDEBUG, " + OpenGL Texture, id [" + pGLTexture.getTextureID() + "], ("+ 
					pGLTexture.getWidth() + " * " + pGLTexture.getHeight() + ")");
			}
		}
		else
		{
			LogManager.getInstance().addMessage(EnumLogType.eWARN, "CGLRenderer::addTexture() : Type de texture non supporté : " + pTexture.getName());
		}
	}
		
	//! Retourne une texture
	AGLTexture getTexture(ATextureDescription pTexture)
	{
		if (pTexture.isValid())
		{
			String textureName = pTexture.getName();
	
			if (!m_Textures.containsKey(textureName))
			{
				addTexture(pTexture);
			}
	
			return m_Textures.get(textureName);
		}
	
		return null;
	}

	//! Delie un materiau
	void releaseMaterial(Material pMaterial)
	{
		if (pMaterial.isBlendingEnabled())
		{
			glDisable(GL_BLEND);
		}

		if (pMaterial.isTwoSided())
		{
			glDisable(GL_CULL_FACE);
		}

		for (MaterialTextureParameter texture : pMaterial.textures())
		{
			ATextureDescription pTexture = TextureManager.getInstance().getTextureByID(texture.getTextureID());

			AGLTexture pGLTexture = getTexture(pTexture);
			if (pGLTexture != null)
			{
				if (pGLTexture.isValid())
				{
					pGLTexture.release();
				}
			}
		}
	}

	//! Fait la correspondance entre un model de shading et un shader
    void linkUserUniformValues(Material pMaterial)
    {
    	// TODO
    	/*
    	if (pMaterial.getShadingModel() == eShadingModelUser)
    	{
    		Shader pShader = m_pSceneManager.getShaderByName(pMaterial.getUserShadingModelName());

    		for (TUniformValue uniformValue : pShader.getUniformValues())
    		{
    			QByteArray name = uniformValue.first.toLatin1();

                switch (int(uniformValue.second.type()))
    			{
    			case QMetaType::Int:
                    m_pCurrentShader.setUniformValue(name.constData(), uniformValue.second.toInt());
    				break;
    			case QMetaType::Float:
                    m_pCurrentShader.setUniformValue(name.constData(), (GLfloat)uniformValue.second.toFloat());
    				break;
    			case QMetaType::Double:
                    m_pCurrentShader.setUniformValue(name.constData(), (GLfloat)uniformValue.second.toDouble());
    				break;
    			case QMetaType::QVector2D:
                    m_pCurrentShader.setUniformValue(name.constData(), uniformValue.second.value<QVector2D>());
    				break;
    			case QMetaType::QVector3D:
                    m_pCurrentShader.setUniformValue(name.constData(), uniformValue.second.value<QVector3D>());
    				break;
    			case QMetaType::QVector4D:
                    m_pCurrentShader.setUniformValue(name.constData(), uniformValue.second.value<QVector4D>());
    				break;
    			case QMetaType::QMatrix4x4:
                    m_pCurrentShader.setUniformValue(name.constData(), uniformValue.second.value<QMatrix4x4>());
                    break;
    			default:
    				break;
    			}
    		}

    		for (TUniformArray& uniformArray, pShader.getUniformValueArrays())
    		{
    			QVariant array = uniformArray.second;
    			QByteArray name = uniformArray.first.toLatin1();

                if (array.canConvert<QVector<float> >())
    			{
                    QVector<float> list = array.value<QVector<float> >();
    				m_pCurrentShader.setUniformValueArray(name.constData(), (GLfloat*)list.constData(), list.size(), 1);
    			}
                else if (array.canConvert<QVector<QVector2D> >())
    			{
                    QVector<QVector2D> list = array.value<QVector<QVector2D> >();
    				m_pCurrentShader.setUniformValueArray(name.constData(), list.constData(), list.size());
    			}
                else if (array.canConvert<QVector<QVector3D> >())
    			{
                    QVector<QVector3D> list = array.value<QVector<QVector3D> >();
    				m_pCurrentShader.setUniformValueArray(name.constData(), list.constData(), list.size());
    			}
                else if (array.canConvert<QVector<QVector4D> >())
    			{
                    QVector<QVector4D> list = array.value<QVector<QVector4D> >();
    				m_pCurrentShader.setUniformValueArray(name.constData(), list.constData(), list.size());
    			}
                else if (array.canConvert<QVector<QMatrix4x4> >())
    			{
                    QVector<QMatrix4x4> list = array.value<QVector<QMatrix4x4> >();
    				m_pCurrentShader.setUniformValueArray(name.constData(), list.constData(), list.size());
    			}
    		}
    	}*/    	
    }


	//! Fait la correspondance entre un model de shading et un shader
    GLShaderProgram getShaderProgram(Material pMaterial)
    {
    	EnumShadingModel eShadingModel = pMaterial.getShadingModel(); 
    	bool bTextured = hasValidTexture(pMaterial);
    	String userShadingModel = pMaterial.getUserShadingModelName();
    	
	    GLShaderProgram pShaderProgram = null;
	
		String shadingModelName;
	
		switch (eShadingModel)
		{
		case eShadingModelUser:
			shadingModelName = userShadingModel;
			break;
		case eShadingModelColor:
			shadingModelName = "color";
			break;
		case eShadingModelPhong:
			if (bTextured)
				shadingModelName = "texturedPhong";
			else shadingModelName = "lightPhong";
			break;
		case eShadingModelToon:
			shadingModelName = "toon";
			break;
		case eShadingModelFresnel:
			shadingModelName = "fresnel";
			break;
		default:
			shadingModelName = "color";
			break;
		}
	
		Shader pShader = m_pSceneManager.getShaderByName(shadingModelName);
		if (pShader != null)
		{
			if (!m_ShaderPrograms.containsKey(pShader.getName()))
			{
				bindShader(pShader);
			}
	
			pShaderProgram = m_ShaderPrograms.get(pShader.getName());
		}
	
		return pShaderProgram;
	}

	//! retourne vrai si le matériau a au moins une texture valide
	bool hasValidTexture(Material pMaterial)
	{
		for (MaterialTextureParameter texture : pMaterial.textures())
		{
			AGLTexture pTexture = getTexture(TextureManager.getInstance().getTextureByID(texture.getTextureID()));
			if (pTexture != null)
			{
				if (pTexture.isValid())
				{
					return true;
				}
			}
		}

		return false;
	}

	
	//! Shader courant
    GLShaderProgram m_pCurrentShader = null;

	//! Temps
    Calendar m_Time = new GregorianCalendar();

    bool isOneVisible(List<ARenderableItem> items)
    {
        for (ARenderableItem pItem : items)
        {
            if (pItem.isVisible())
                return true;
        }

        return false;
    }

	int m_iDrawCalls = 0;
	int m_iPolygonsPerFrame = 0;
	int m_iMaterialBind = 0;
	int m_iShaderBind = 0;

}

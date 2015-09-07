package opengl;

import geometry.global.Global;
import geometry.material.EnumMaterialParameter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import tools.types.Pair;

class GLHelper {
	
	static GLHelper getInstance()
	{
		if (s_pInstance == null)
		{
			s_pInstance = new GLHelper();
		}

		return s_pInstance;
	}

	String getLightParamStr(Pair<Integer, EnumLightParameter> param)
	{
		return m_LightParamStr.get(param);
	}

	String fromType(EnumMaterialParameter eType)
	{
		return m_MaterialParameterName.get(eType.ordinal());
	}

	String fromTypeCount(EnumMaterialParameter eType)
	{
		return m_MaterialParameterCountName.get(eType.ordinal());
	}

	//! Constructeur
	GLHelper()
	{
		for (int i = 0; i < Global.MaxLightCount; ++i)
		{
			for (EnumLightParameter e : EnumLightParameter.values())
			{
				m_LightParamStr.put(new Pair<Integer, EnumLightParameter>(i, e), fromLightParam(i, e));
			}
		}
	
		m_MaterialParameterName.add("texture");
		m_MaterialParameterName.add("textureDiffuse");
		m_MaterialParameterName.add("textureSpecular");
		m_MaterialParameterName.add("textureAmbient");
		m_MaterialParameterName.add("textureEmissive");
		m_MaterialParameterName.add("textureHeight");
		m_MaterialParameterName.add("textureNormals");
		m_MaterialParameterName.add("textureShininess");
		m_MaterialParameterName.add("textureOpacity");
		m_MaterialParameterName.add("textureDisplacement");
		m_MaterialParameterName.add("textureLightmap");
		m_MaterialParameterName.add("textureReflection");
		
		m_MaterialParameterCountName.add("textureCount");
		m_MaterialParameterCountName.add("textureDiffuseCount");
		m_MaterialParameterCountName.add("textureSpecularCount");
		m_MaterialParameterCountName.add("textureAmbientCount");
		m_MaterialParameterCountName.add("textureEmissiveCount");
		m_MaterialParameterCountName.add("textureHeightCount");
		m_MaterialParameterCountName.add("textureNormalsCount");
		m_MaterialParameterCountName.add("textureShininessCount");
		m_MaterialParameterCountName.add("textureOpacityCount");
		m_MaterialParameterCountName.add("textureDisplacementCount");
		m_MaterialParameterCountName.add("textureLightmapCount");
		m_MaterialParameterCountName.add("textureReflectionCount");
	}

	HashMap<Pair<Integer, EnumLightParameter>, String> m_LightParamStr = new HashMap<Pair<Integer, EnumLightParameter>, String>(); 

	static String fromLightParam(int iLightID, EnumLightParameter eParam)
	{
		switch (eParam)
		{
		case eIsEnabled				: return String.format("lights[%1].enabled", iLightID);
		case ePosition				: return String.format("lights[%1].position", iLightID);
		case eDirection				: return String.format("lights[%1].direction", iLightID);
		case eAmbientColor			: return String.format("lights[%1].ambient", iLightID);
		case eDiffuseColor			: return String.format("lights[%1].diffuse", iLightID);
		case eSpecularColor			: return String.format("lights[%1].specular", iLightID);
		case eConstantAttenuation	: return String.format("lights[%1].constantAttenuation", iLightID);
		case eLinearAttenuation		: return String.format("lights[%1].linearAttenuation", iLightID);
		case eQuadraticAttenuation	: return String.format("lights[%1].quadraticAttenuation", iLightID);
		}
		return null;
	}

	static GLHelper s_pInstance = null;
	
	List<String> m_MaterialParameterName = new ArrayList<String>();
	List<String> m_MaterialParameterCountName = new ArrayList<String>();
}

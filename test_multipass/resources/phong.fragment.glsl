#version 330

in vec3 texCoord;
in vec3 normal;
in vec3 position;

struct LightITS
{
	vec3 direction;
	vec3 viewDir;
};

in LightITS lightsITS[8];


uniform float factor = 1.0;

uniform int textureDiffuseCount;
uniform int textureOpacityCount;
uniform int textureNormalsCount;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureOpacity0;
uniform sampler2D textureNormals0;

// uniform inputs
struct Light
{
	bool enabled;
	vec4 position;  // Light position in eye coords.
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform Light lights[8];
uniform int lightCount;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 alphaMask;
	int alphaMaskFunc;
	float shininess;
	float opacity;
};

uniform Material material;

// outputs
out vec4 fragColor;


void phongModel(const in int lightIndex, vec3 pos, vec3 norm, out vec3 ambientAndDiff, out vec3 spec)
{
	vec3 s = normalize(vec3(lights[lightIndex].position) - pos);
	vec3 v = normalize(-pos.xyz);

	vec3 r = reflect(-s, norm);

	// Calculate the ambient contribution
	vec3 ambient = vec3(lights[lightIndex].ambient * material.ambient);

	// Calculate the diffuse contribution
	float sDotN = max(dot(s, norm), 0.0);
	vec3 diffuse = vec3(lights[lightIndex].diffuse * material.diffuse) * sDotN;

	// Sum the ambient and diffuse contributions
	ambientAndDiff = ambient + diffuse;

	// Calculate the specular highlight component
	spec = vec3(0.0);
	if (sDotN > 0.0)
	{
		spec = vec3(lights[lightIndex].specular * material.specular) * pow(max(dot(r, v), 0.0), material.shininess);
	}
}

void main()
{
	if (textureOpacityCount > 0)
	{
		vec4 alphaColor = texture(textureOpacity0, vec2(texCoord));

		if (material.alphaMaskFunc == 0)		// eAlways
		{
			discard;
		}
		else if (material.alphaMaskFunc == 1)	// eLess
		{
			if (alphaColor.r > material.alphaMask.r && alphaColor.g > material.alphaMask.g && alphaColor.b > material.alphaMask.b) 
				discard;
		}
		else if (material.alphaMaskFunc == 2)	// eEqual
		{
			if (alphaColor == material.alphaMask)
				discard;
		}
		else if (material.alphaMaskFunc == 3)	// eLessOrEqual
		{
			if (alphaColor.r >= material.alphaMask.r && alphaColor.g >= material.alphaMask.g && alphaColor.b >= material.alphaMask.b)
				discard;
		}
		else if (material.alphaMaskFunc == 4)	// eGreater
		{
			if (alphaColor.r < material.alphaMask.r && alphaColor.g < material.alphaMask.g && alphaColor.b < material.alphaMask.b)
				discard;
		}
		else if (material.alphaMaskFunc == 5)	// eNotEqual
		{
			if (alphaColor != material.alphaMask)
				discard;
		}
		else if (material.alphaMaskFunc == 6)	// eGreaterOrEqual
		{
			if (alphaColor.r <= material.alphaMask.r && alphaColor.g <= material.alphaMask.g && alphaColor.b <= material.alphaMask.b)
				discard;
		}
		// else eAlaways : nothing to do
	}
    
	vec4 textureColor;

	if (textureDiffuseCount > 0)
	{
		// Sample the textures at the interpolated texCoords
		if (textureDiffuseCount == 2)
		{
			vec4 texture0Color = texture(textureDiffuse0, vec2(texCoord));
			vec4 texture1Color = texture(textureDiffuse1, vec2(texCoord));
			textureColor = mix(texture0Color, texture1Color, 0.3);
		}
		else
		{
			textureColor = texture(textureDiffuse0, vec2(texCoord));
		}
	}

	vec4 color = vec4(0.0);
	for (int i = 0; i < lightCount; ++i)
	{
		if (lights[i].enabled)
		{
			// Calculate the lighting model, keeping the specular component separate
			vec3 ambientAndDiff, spec;
    
			phongModel(i, position, normal, ambientAndDiff, spec);
		
			if (textureDiffuseCount > 0)
			{
				color += (vec4(ambientAndDiff, 1.0) * textureColor + vec4(spec, 1.0));
			}
			else
			{
				color += (vec4(ambientAndDiff, 1.0) * material.diffuse + vec4(spec, 1.0));
			}
		}
	}

	// Calculate the final fragment color by multiplying the ambient and diffuse color
	// by the texture colour and then adding on the specular highlight contribution

	fragColor = vec4(color.xyz, material.opacity);
}



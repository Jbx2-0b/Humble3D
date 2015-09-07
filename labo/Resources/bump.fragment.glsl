#version 330

in vec3 lightDir;
in vec2 texCoord;
in vec3 viewDir;

uniform sampler2D textureDiffuse0;
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

uniform Light lights[1];

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float opacity;
};

uniform Material material;

layout( location = 0 ) out vec4 FragColor;


vec3 phongModel(vec3 norm, vec3 diffR)
{
	vec3 r = reflect(-lightDir, norm);
	vec3 ambient = vec3(lights[0].diffuse * material.ambient);
	float sDotN = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = lights[0].diffuse.xyz * diffR * sDotN;
	
	vec3 spec = vec3(0.0);
	if (sDotN > 0.0)
		spec = vec3(lights[0].diffuse * material.specular * pow(max(dot(r, viewDir), 0.0),	material.shininess));
		
	return ambient + diffuse + spec;

	return vec3(0.0, 0.0, 1.0);
}

void main()
{
	// Lookup the normal from the normal map
	vec4 normal = texture(textureNormals0, texCoord);
	
	// The color texture is used as the diffuse reflectivity
	vec4 texColor = texture(textureDiffuse0, texCoord.xy);
	
	FragColor = vec4(phongModel(normal.xyz, texColor.rgb), 1.0);
}
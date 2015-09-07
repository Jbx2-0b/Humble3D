#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

in vec4 vertexTangent;

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

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

out vec3 lightDir;
out vec2 texCoord;
out vec3 viewDir;


void main()
{
	// Transform normal and tangent to eye space
	vec3 norm = normalize(normalMatrix * vertexNormal);
	vec3 tang = normalize(normalMatrix * vec3(vertexTangent));
	
	// Compute the binormal
	vec3 binormal = normalize(cross( norm, tang)) * vertexTangent.w;
	
	// Matrix for transformation to tangent space
	mat3 toObjectLocal = mat3(tang.x, binormal.x, norm.x,
							  tang.y, binormal.y, norm.y,
							  tang.z, binormal.z, norm.z);
							  
	// Get the position in eye coordinates
	vec3 pos = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
	
	// Transform light dir. and view dir. to tangent space
	lightDir = normalize(toObjectLocal * (lights[0].position.xyz - pos));
	viewDir = toObjectLocal * normalize(-pos);
	
	// Pass along the texture coordinate
	texCoord = vertexTexCoord.xy;
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}
#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

out vec3 texCoord;
out vec3 normal;
out vec3 position;
out vec4 lightDir; 
out vec4 eyeVec; 

uniform mat4 modelViewProjectionMatrix; 
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix; 

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
 

void main() 
{ 
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
    texCoord = vertexTexCoord; 
    normal = normalMatrix  * vertexNormal;
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    vec4 viewVertex = modelViewMatrix * vec4(vertexPosition, 1.0);
    lightDir = lights[0].position - viewVertex; 
    eyeVec = -viewVertex; 
}




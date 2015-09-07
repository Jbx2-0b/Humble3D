#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;

// uniform inputs
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

// outputs
out vec3 vsTexCoord;
out vec3 vsNormal;
out vec3 vsLocalNormal;
out vec3 vsPosition;
out vec3 vsLocalPosition;

void main()
{
    vsTexCoord = vertexTexCoord;
    vsNormal = normalize(normalMatrix * vertexNormal);
    vsLocalNormal = normalize(vertexNormal);
    vsPosition = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    vsLocalPosition = vertexPosition;
    
    // Calculate vertex position in screen space
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

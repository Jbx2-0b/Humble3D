#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 position;
out vec3 normal;
out float altitude;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;

void main()
{
    altitude = vertexPosition.y;
    normal = normalize(normalMatrix * vertexNormal);
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

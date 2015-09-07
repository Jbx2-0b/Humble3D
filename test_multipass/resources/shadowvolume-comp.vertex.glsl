#version 430

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 Position;
out vec3 Normal;

// uniform inputs
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

void main()
{
    Normal = normalize(normalMatrix * vertexNormal);
    Position = vec3(modelViewMatrix * vec4(vertexPosition,1.0));

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1.0);
}

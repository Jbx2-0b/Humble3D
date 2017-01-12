#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

out vec3 vsLocalPosition;

void main()
{
    vsLocalPosition = vertexPosition;
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

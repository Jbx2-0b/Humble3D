#version 330

layout (location = 0) in vec4 vertexPosition;

uniform mat4 modelViewMatrix;

// ouputs
out vec4 vsPosition;

void main()
{
    vsPosition = modelViewMatrix * vertexPosition;
}
#version 330

uniform mat4 modelViewProjectionMatrix;

layout (location = 0) in vec3 vertexPosition;

void main()
{    
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}



#version 330

// inputs
layout (location = 0) in vec4 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

void main()
{         
    // Calculate vertex position in screen space
    gl_Position = modelViewProjectionMatrix * vertexPosition;
}



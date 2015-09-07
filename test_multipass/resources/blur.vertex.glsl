#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexTexCoord;


// uniform inputs
uniform mat4 modelViewProjectionMatrix;
   
// outputs
out vec3 texCoord;

void main()
{
	texCoord = vertexTexCoord;
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}